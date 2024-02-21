#include <stdio.h>
#include <stdlib.h>

#include "grapheCycles.h"
#include "TriFusion.h"
#include "utiles.h"

grapheCycles transfoGrapheCycles(grapheMol graphe_mol, listeCycles *cycles, indexCycles *index_cycles) {

  int nb_aretes = 0;
  listeAretes *aretes = NULL;
  grapheCycles graphe_cycles = initGrapheCycles(graphe_mol.chebi_id, cycles);

  ajouterAreteCyclesDisjoints(cycles, &aretes, &nb_aretes, index_cycles, graphe_mol);
  //printIndexCycles(index_cycles, 30);
  ajouterAreteCyclesJoints(cycles, &aretes, &nb_aretes);
  ajouterAreteDansGraphe(&graphe_cycles, aretes, nb_aretes);

  return graphe_cycles;
}

// Marquer à 2 une arête dans la matrice d'adjacence de g si elle appartient à un cycle.
// Les arêtes qui restent marquées à 1 sont des isthmes.
// Remplie également un tableau d'index contenant les id des cycles auxquels appartient chaque sommet.
// A la fin, index_cycles[i].cycles est un tableau qui contient :
// + des -1 si le sommet i n'appartient à aucun cycle de cycles
// + l'id des cycles sinon.
void marquerAretesCycles(grapheMol *g, listeCycles *cycles, indexCycles *index_cycles) {

  int i, j;
  int id1, id2;
  cycle c;
  
  for (i = 0; i < cycles->nb_cycles; i++) {

    c = cycles->cycles[i];
    id1 = c.sommets[0];
    id2 = c.sommets[c.taille - 1];
    if (g->adjacence[id1][id2] || g->adjacence[id2][id1]) {
      g->adjacence[id1][id2] = 2;
      g->adjacence[id2][id1] = 2;
    }
    for (j = 0; j < c.taille - 1; j++) {

      ajouterCycleDansIndex(index_cycles, c.sommets[j], i);

      id1 = c.sommets[j];
      id2 = c.sommets[j + 1];
      if (g->adjacence[id1][id2] || g->adjacence[id2][id1]) {
        g->adjacence[id1][id2] = 2;
        g->adjacence[id2][id1] = 2;
      }
    }
    ajouterCycleDansIndex(index_cycles, c.sommets[c.taille - 1], i);
  }
}

// Trie les sommets de tous les cycles contenus dans cycles par identifiant croissant.
void trierSommetsCycles(listeCycles *cycles) {

  int i;
  for (i = 0; i < cycles->nb_cycles; i++) {
    triParInsertionSommets(cycles->cycles[i]);
  }
}

// Nombre de sommets en commun entre les cycles c1 et c2
int intersectionCycles(cycle c1, cycle c2) {

  int id1, id2, n1, n2, intersection;
  n1 = c1.taille;
  n2 = c2.taille;
  intersection = 0;
  id1 = 0, id2 = 0;

  while (id1 < n1 && id2 < n2) {
    if (c1.sommets[id1] == c2.sommets[id2]) {
      id1++;
      id2++;
      intersection++;
    }
    else if (c1.sommets[id1] < c2.sommets[id2]) {
      id1++;
    }
    else {
      id2++;
    }
  }
  return intersection;
}

// Compare deux-à-deux les cycles dans cycles, et ajoute une arête dans aretes
// entre c1 et c2 si l'intersection des sommets des cycles est non vide.
// nb_aretes sauvegarde le nombre d'arêtes ajoutées.
void ajouterAreteCyclesJoints(listeCycles *cycles, listeAretes **aretes, int *nb_aretes) {

  int i, j, sommets_communs, aretes_communes;

  trierSommetsCycles(cycles);

  for (i = 0; i < cycles->nb_cycles; i++) {
    for (j = i + 1; j < cycles->nb_cycles; j++) {
      sommets_communs = intersectionCycles(cycles->cycles[i], cycles->cycles[j]);
      if (sommets_communs) {
        aretes_communes = sommets_communs - 1;
        ajouterAreteDansListe(aretes, nb_aretes, i, j, 1, aretes_communes);
      }
    }
  }
}

void ajouterAreteEntreCycles(int id1, int id2, int taille, listeAretes **aretes, int *nb_aretes, indexCycles *index_cycles) {
  
  int i, j, c1, c2;
  i = 0;
    
  // Pour tous les cycles auxquels appartient id2
  while (i < SIZE_INDEX && index_cycles[id2].cycles[i] != -1) {
    c2 = index_cycles[id2].cycles[i];
    j = 0;
    // Pour tous les cycles auxquels appartient id1
    while (j < SIZE_INDEX && index_cycles[id1].cycles[j] != -1) {
      c1 = index_cycles[id1].cycles[i];
      ajouterAreteDansListe(aretes, nb_aretes, c1, c2, 2, taille);
      j++;
    }
    i++;
  }
}

// Parcourt les arêtes du graphe appartenant à une chaîne et dont l'extrémité se termine dans un cycle.
// A l'appel de la fonction, id_debut doit être égal à id_1. Le parcours commence
// par l'arête id1-id2 au première appel de la fonction, puis la fonction
// explore récurssivement toutes les chaînes commençant par id1-id2 en passant 
// par les arêtes marquées 1 (=isthmes) dans la matrice d'adjacence du graphe.
// Quand id2 appartient à un cycle, ajoute une arête entre tous les cycles contenant 
// le sommet id_debut et tous ceux contenant le sommet id2, dans la liste aretes.
// Le poids de l'arête correspond à la taille de la chaine parcourue.
// Enfin, l'arête est marquée comme traitée (-1 dans la partie triangulaire inférieure de la matrice d'adjacence).
void parcoursChaine(int id_debut, int id1, int id2, int taille, listeAretes **aretes, int *nb_aretes, indexCycles *index_cycles, grapheMol g, int max) {

  if (max-- < 0) {
    printf("Chebi %d :le parcours des isthmes prends trop de temps...\n", g.chebi_id);
    return;
  }
  // Si Sommet id2 appartient à un cycle
  if (index_cycles[id2].cycles[0] > -1) {
    ajouterAreteEntreCycles(id_debut, id2, taille, aretes, nb_aretes, index_cycles);
  }
  // Sinon, aller voir tous les sommets voisins de id2
  else {
    int j;
    // Pour tous les voisins différents de id1 et qui appartiennent à un isthme
    for (j = 0; j < id2; j++) {
      if (j!= id1 && g.adjacence[j][id2] == 1) {
        parcoursChaine(id_debut, id2, j, taille + 1, aretes, nb_aretes, index_cycles, g, max);
      }
    }
    for (j = id2 + 1; j < g.nb_sommets; j++) {
      if (j!= id1 && g.adjacence[id2][j] == 1) {
        parcoursChaine(id_debut, id2, j, taille + 1, aretes, nb_aretes, index_cycles, g, max);
      }
    } 
  }
  // Marquer l'isthme (arête id1-id2) comme vu
  if (id1 < id2) {
    g.adjacence[id2][id1] = -1;
  }
  else {
    g.adjacence[id1][id2] = -1;
  }
}

// Parcourt les isthmes dans la matrice d'adjacence de g, et teste :
// + si l'isthme est partagée entre deux cycles, ajoute une arête dans aretes.
// + si l'isthme a une seule extrémité dans un cycle, fait un parcours sur
// les arêtes et s'arrête sur les premiers sommets appartenant à un cycle. 
//
// index_cycles donne l'id des cycles auxquels appartient le sommet i, sinon -1.
// La partie triangulaire inférieure de la matrice d'adjacence de g est utilisée
// pour marquer (-1) les isthmes qui ont déjà été vus.
void ajouterAreteCyclesDisjoints(listeCycles *cycles, listeAretes **aretes, int *nb_aretes, indexCycles *index_cycles, grapheMol g) {

  int i, j, max = 100;

  marquerAretesCycles(&g, cycles, index_cycles);

  printGrapheMol(g);

  for (i = 0; i < g.nb_sommets; i++) {
    for (j = i + 1; j < g.nb_sommets; j++) {
      // Pour tous les isthmes non marqués (= déjà vus)
      if (g.adjacence[i][j] == 1 && g.adjacence[j][i] != -1) {
        // Chaine de taille 1 entre deux cycles
        if (index_cycles[i].cycles[0] > -1 && index_cycles[j].cycles[0] > -1) {
          ajouterAreteEntreCycles(i, j, 1, aretes, nb_aretes, index_cycles);
        }
        // Sommet i appartient à un cycle
        else if (index_cycles[i].cycles[0] > -1 && index_cycles[j].cycles[0] == -1) {
          parcoursChaine(i, i, j, 1, aretes, nb_aretes, index_cycles, g, max);
        }
        // Sommet j appartient à un cycle
        else if (index_cycles[i].cycles[0] == -1 && index_cycles[j].cycles[0] > -1) {
          parcoursChaine(j, j, i, 1, aretes, nb_aretes, index_cycles, g, max);
        }
      }
    }
  }
}


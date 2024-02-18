#include <stdio.h>
#include <stdlib.h>

#include "grapheCycles.h"
#include "TriFusion.h"


// Marquer à 2 une arête dans la matrice d'adjacence de g
// si elle appartient à un cycle.
// Les arêtes qui restent marquées à 1 sont des isthmes.
// TODO le faire pendant l'exécution de l'algo d'Horton
void marquerAretesCycles(grapheMol *g, listeCycles *liste_c) {

  int i, j, n;
  int id1, id2;
  Cycle c;
  n = liste_c->nb_cycles;
  
  for (i = 0; i < n; i++) {
    c = liste_c->cycles[i];
    id1 = c.sommets[0];
    id2 = c.sommets[c.taille - 1];
    if (g->adjacence[id1][id2] || g->adjacence[id2][id1]) {
      g->adjacence[id1][id2] = 2;
      g->adjacence[id2][id1] = 2;
    }
    for (j = 0; j < c.taille - 1; j++) {
      id1 = c.sommets[j];
      id2 = c.sommets[j + 1];
      if (g->adjacence[id1][id2] || g->adjacence[id2][id1]) {
        g->adjacence[id1][id2] = 2;
        g->adjacence[id2][id1] = 2;
      }
    }
  }
}

// Si les arêtes du graphe appartenant à un cycle sont marquées 2,
// retourne les isthmes (marquées 1), et sauvegarde leur nombre dans nb_isthmes.
listeAretes* trouverIsthmes(grapheMol g, int *nb_isthmes) {

  listeAretes *isthmes = NULL;
  int i, j;
  for (i = 0; i < g.nb_sommets; i++) {
    for (j = i + 1; j < g.nb_sommets; j++) {
      if (g.adjacence[i][j] == 1) {
        ajouterAreteDansListe(&isthmes, nb_isthmes, i, j, 0, 0);
      }
    }
  }
  return isthmes;
}

// Trie les sommets de tous les cycles contenus dans liste_c par identifiant croissant.
// Remplie également un tableau d'index contenant les id des cycles auxquels appartient chaque sommet.
// A la fin, index_cycles[i].cycles est un tableau qui contient :
// + des -1 si le sommet i n'appartient à aucun cycle de liste_c
// + l'id des cycles sinon.
void trierSommetsCycles(listeCycles *liste_c, indexCycles *index_cycles) {

  int i, n;
  n = liste_c->nb_cycles;

  for (i = 0; i < n; i++) {
    triParInsertionSommets(liste_c->cycles[i], i, index_cycles);
  };
}

// Nombre de sommets en commun entre les cycles c1 et c2
int intersectionCycles(Cycle c1, Cycle c2) {

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

// Compare deux-à-deux les cycles dans liste_c, et ajoute une arête dans aretes
// entre c1 et c2 si l'intersection des sommets des cycles est non vide.
// nb_aretes sauvegarde le nombre d'arêtes ajoutées.
void ajouterAreteCyclesJoints(listeCycles *liste_c, listeAretes **aretes, int *nb_aretes) {

  int i, j, sommets_communs, aretes_communes;
  for (i = 0; i < liste_c->nb_cycles; i++) {
    for (j = i + 1; j < liste_c->nb_cycles; j++) {
      sommets_communs = intersectionCycles(liste_c->cycles[i], liste_c->cycles[j]);
      if (sommets_communs) {
        aretes_communes = sommets_communs - 1;
        ajouterAreteDansListe(aretes, nb_aretes, i, j, 1, aretes_communes);
      }
    }
  }
}

// Parcourt les arêtes du graphe appartenant à une chaîne et dont l'extrémité se termine dans un cycle.
// A l'appel de la fonction, id_debut doit être égal à id_1. Le parcours commence
// donc par l'arête id1-id2 au première appel de la fonction, puis la fonction
// explore récurssivement toutes les chaînes commençant par id1-id2 en passant 
// par les arêtes marquées 1 (=isthmes) dans la matrice d'adjacence du graphe.
// Quand id2 appartient à un cycle, ajoute une arête, dans la liste aretes,
// entre tous les cycles contenant le sommet id_debut et tous les cycles contenant
// le sommet id2. Le poids de l'arête correspond à la taille de la chaine parcourue.
void parcoursChaine(int id_debut, int id1, int id2, int taille, listeAretes **aretes, int *nb_aretes, indexCycles *index_cycles, grapheMol g) {

  // Si Sommet id2 appartient à un cycle
  if (index_cycles[id2].cycles[0] > -1) {
    int i, j;
    int c1, c2;

    i = 0;
    // Pour tous les cycles auxquels appartient id2
    while (i < SIZE_INDEX && index_cycles[id2].cycles[i] != -1) {
      c2 = index_cycles[id2].cycles[i];
      j = 0;
      // Pour tous les cycles auxquels appartient id_debut
      while (j < SIZE_INDEX && index_cycles[id_debut].cycles[j] != -1) {
        c1 = index_cycles[id_debut].cycles[i];
        ajouterAreteDansListe(aretes, nb_aretes, c1, c2, 2, taille);
        j++;
      }
      i++;
    }
    // Marquer le sommet id1 comme vu
    index_cycles[id1].cycles[0] = -2;
  }
  // Sinon, aller voir tous les sommets voisins de id2
  else {
    int j;
    for (j = 0; j < g.nb_sommets; j++) {
      // Le voisin est différent de id1 et est l'extrémité d'un isthme
      if (j!= id1 && g.adjacence[id2][j] == 1) {
        parcoursChaine(id_debut, id2, j, taille + 1, aretes, nb_aretes, index_cycles, g);
      }
    } 
  }
  return; // Seulement si condition remplie
}

// Récupère la liste des isthmes du graphe g, la parcourt et teste :
// + si l'isthme est partagée entre deux cycles, ajoute une arête dans aretes.
// + si l'isthme a une seule extrémité dans un cycle, fait un parcours sur
// les arêtes et s'arrête sur les premiers sommets appartenant à un cycle. 
//
// index_cycles donne l'id des cycles auxquels appartient le sommet i, sinon -1.
// index_cycles est aussi utilisé pour marquer les sommets appartenant à un isthme
// qui sont déjà traités. Ils sont marqués -2 (voir dans parcoursChaine).
void ajouterAreteCyclesDisjoints(listeCycles *liste_c, listeAretes **aretes, int *nb_aretes, indexCycles *index_cycles, grapheMol g) {

  int nb_isthmes = 0;
  int id1, id2;

  marquerAretesCycles(&g, liste_c);
  listeAretes *isthmes = trouverIsthmes(g, &nb_isthmes);
  listeAretes *temp;

  while (isthmes) {
    id1 = isthmes->a.id1;
    id2 = isthmes->a.id2;
    // Chaine de taille 1 entre deux cycles
    if (index_cycles[id1].cycles[0] > -1 && index_cycles[id2].cycles[0] > -1) {
      ajouterAreteDansListe(aretes, nb_aretes, id1, id2, 2, 1);
    }
    // Isthme au milieu d'une chaîne ou déjà vue
    //else if (index_cycles[id1].cycles[0] == -1 && index_cycles[id1].cycles[0] == -1) {}

    else if (index_cycles[id1].cycles[0] > -1 && index_cycles[id2].cycles[0] == -1) {
      parcoursChaine(id1, id1, id2, 1, aretes, nb_aretes, index_cycles, g);
    }
    else if (index_cycles[id1].cycles[0] == -1 && index_cycles[id2].cycles[0] > -1) {
      parcoursChaine(id2, id2, id1, 1, aretes, nb_aretes, index_cycles, g);
    }

    temp = isthmes;
    isthmes = isthmes->suiv;
    free(temp);
  }
}


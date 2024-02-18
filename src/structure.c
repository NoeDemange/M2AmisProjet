#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "structure.h"
#include "utiles.h"
#include "McKay.h"

void procedure(char *nom_dossier, int max_fichiers) {
  
  listeFichiers *fichiers = lireDossier(nom_dossier, max_fichiers);
  if (max_fichiers > 0)
    printListeFichiers(fichiers);
  
  while (fichiers) {

    grapheMol g = lireFichier(nom_dossier, fichiers->nom);

    // TODO tester si g a plus de 3 sommets.

    if (max_fichiers > 0) {
      printGrapheMol(g);
      printf("McKay\n");
    }

    grapheCanonique(&g);

    if (max_fichiers > 0)
    printGrapheMol(g);

    fichiers = freeListeFichiers(fichiers);
    freeGrapheMol(g);
  }
}
 
 // Scanne le dossier nom_dossier et stocke le nom de max_fichiers fichiers dans une liste.
 // Si max_fichiers <= 0, stocke tous les fichiers du dossier (sauf ".", "..").
listeFichiers* lireDossier(char *nom_dossier, int max_fichiers) {
  
  struct dirent *dir;
  DIR *d = opendir(nom_dossier);
  listeFichiers *fichiers = initListeFichiers();
  int iter = 0;

  if (d) {
    while ((dir = readdir(d)) != NULL && (iter < max_fichiers || max_fichiers <= 0)) {

      size_t taille_allouee = strlen(dir->d_name) ;
      char *nom_fichier = (char *)malloc(taille_allouee);
      strcpy(nom_fichier, dir->d_name);
      if (strcmp(".", nom_fichier) && strcmp("..", nom_fichier)) {
        ajouterNomFichier(&fichiers, nom_fichier);
        iter++;
      }
    }
    closedir(d);
  }
  printf("Nombre de fichiers : %d\n", iter);
  return fichiers;
}

// Scanne le fichier nom_fichier et stocke le contenu de la matrice d'adjacence dans un grapheMol.
// Le nom du fichier doit être le numéro du ChEBI id.
grapheMol lireFichier(char* nom_dossier, char *nom_fichier) {
  
  FILE *f;
  grapheMol g;
  int nb_sommets, premier, valeur;
  int i, j;

  char path[264];
  sprintf(path, "%s/%s", nom_dossier, nom_fichier);

  f = fopen(path, "r");
  verifScan(fscanf(f, "%d", &nb_sommets), nom_fichier);

  g = initGrapheMol(nb_sommets, atoi(nom_fichier));

  for (i = 0; i < nb_sommets; i++) {
    verifScan(fscanf(f, "%d", &premier), nom_fichier);
    for (j = i + 1; j < nb_sommets; j++) {
      verifScan(fscanf(f, " %d", &valeur), nom_fichier);
      g.adjacence[i][j] = valeur;
      g.adjacence[j][i] = valeur;
    }
  }
  fclose(f);

  return g;
}

grapheMol initGrapheMol(int nb_sommets, int chebi_id) {

  grapheMol g;
  g.nb_sommets = nb_sommets;
  g.chebi_id = chebi_id;
  g.adjacence = malloc(nb_sommets * sizeof(int*));

  for(int i = 0; i < nb_sommets; i++) {
    g.adjacence[i] = calloc(nb_sommets, sizeof(int));
  }

  return g;
}

void freeGrapheMol(grapheMol g) {

  for(int i = 0; i < g.nb_sommets; i++) {
    free(g.adjacence[i]);
  }
  free(g.adjacence);
}

void printGrapheMol(grapheMol g) {
  
  printf("matrice adj de %d : %d\n", g.chebi_id, g.nb_sommets);
  for (int i = 0; i < g.nb_sommets; i++) {
    for (int j = 0; j < g.nb_sommets; j++) {
      printf(" %d", g.adjacence[i][j]);
    }
    printf("\n");
  }
}

listeFichiers* initListeFichiers(void) {

  listeFichiers *f = malloc(sizeof(struct listeFichiers));
  f->nom = NULL;
  f->suiv = NULL;
  return f;
}

void ajouterNomFichier(listeFichiers **fichiers, char *nom_fichier) {

  if (fichiers && (*fichiers)->nom == NULL) {
    (*fichiers)->nom = nom_fichier;
  }
  else {
    listeFichiers *nouveau = malloc(sizeof(struct listeFichiers));
    nouveau->nom = nom_fichier;
    nouveau->suiv = *fichiers;
    *fichiers = nouveau;
  }
}

listeFichiers* freeListeFichiers(listeFichiers *fichier) {
  
  if (fichier) {
    listeFichiers *suiv;
    suiv = fichier->suiv;
    free(fichier->nom);
    free(fichier);
    return suiv;
  }
  return NULL;
}

void printListeFichiers(listeFichiers *fichiers) {
  
  if (fichiers) {
    printf("%s", fichiers->nom);
    listeFichiers *fichier = fichiers->suiv;
    while(fichier) {
      printf(", %s", fichier->nom);
      fichier = fichier->suiv;
    }
    printf("\n");
  }
}

indexCycles* initIndexCycles(int taille) {

  indexCycles *index_cycles = malloc(taille * sizeof(indexCycles));
  index_cycles->taille = taille;
  int i, j;

  for (i = 0; i < taille; i++) {
    index_cycles[i].cycles = malloc(SIZE_INDEX * sizeof(int));

    for (j = 0; j < SIZE_INDEX; j++) {
      index_cycles[i].cycles[j] = -1;
    }
  }

  return index_cycles;
}

void resetIndexCycles(indexCycles *index_cycles, int taille) {

  int i, j;
  for (i = 0; i < taille; i++) {
    j = 0;
    while (j < SIZE_INDEX && index_cycles[i].cycles[j] != -1) {
      index_cycles[i].cycles[j] = -1;
      j++;
    }
  }
}

void ajouterCycleDansIndex(indexCycles *index_cycles, int id_sommet, int id_cycle) {
    
  if (id_sommet > index_cycles->taille) {
    printf("Erreur : la taille de buffer allouée pour indexer les cycles");
    printf(" est inférieure à la taille du graphe moléculaire.\n");
    exit(EXIT_FAILURE);
  }

  int i = 0;
  while (i < SIZE_INDEX && index_cycles[id_sommet].cycles[i] != -1) {
    i++;
  }
  if (i == SIZE_INDEX) {
    // TODO faire un realloc
    printf("Erreur interne : taille de buffer SIZE_INDEX est trop petite.\n");
    exit(EXIT_FAILURE);
  }
  else {
    index_cycles[id_sommet].cycles[i] = id_cycle;
  }
}

void freeIndexCycles(indexCycles *index_cycles) {

  int i ;
  for (i = 0; i < index_cycles->taille; i++) {
    free(index_cycles[i].cycles);
  }
  free(index_cycles);
}

void printIndexCycles(indexCycles *index_cycles, int taille) {

  int i, j;

  if (taille <= 0) {
    taille = index_cycles->taille;
  }
  for (i = 0; i < taille; i++) {
    printf("%d :", i);
    j = 0;
    while (j < SIZE_INDEX && index_cycles[i].cycles[j] != -1) {
      printf(" c%d", index_cycles[i].cycles[j]);
      j++;
    }
    printf("\n");
  }
}

grapheCycles initGrapheCycles(listeCycles *liste_c) {

  grapheCycles g;
  g.nb_sommets = liste_c->nb_cycles;
  g.nb_aretes = 0;
  g.aretes = NULL;
  g.sommets = malloc(liste_c->nb_cycles * sizeof(sommet));
  int i;

  for (i = 0; i < liste_c->nb_cycles; i++) {
    g.sommets[i].id = i;
    g.sommets[i].taille = liste_c->cycles[i].taille;
  }

  return g;
}

arete initArete(int id1, int id2, int type, int poids) {

  arete a;
  a.id1 = id1;
  a.id2 = id2;
  a.type = type;
  a.poids = poids;

  return a;
}

// Crée une arête entre les sommets id1 et id2 et l'ajoute à la liste aretes.
// Initialise la liste aretes si elle est vide.  
void ajouterAreteDansListe(listeAretes **aretes, int *nb_aretes, int id1, int id2, int type, int poids) {

  if (*aretes == NULL) {
    *aretes = malloc(sizeof(listeAretes));
    (*aretes)->a = initArete(id1, id2, type, poids);
    (*aretes)->suiv = NULL;
  }
  else {
    listeAretes *nouvelle = malloc(sizeof(listeAretes));
    nouvelle->a = initArete(id1, id2, type, poids);
    nouvelle->suiv = *aretes;
    *aretes = nouvelle;
  }
  (*nb_aretes)++;
}

void freeListeAretes(listeAretes *aretes) {

  listeAretes *temp;
  while (aretes) {
    temp = aretes;
    aretes = aretes->suiv;
    free(temp);
  }
}

// Insère les arêtes de la liste dans le tableau aretes de g,
// et libère la mémoire allouée à la liste.  
void ajouterAreteDansGraphe(grapheCycles *g, listeAretes *aretes, int nb_aretes) {

  g->nb_aretes = nb_aretes;
  g->aretes = malloc(nb_aretes * sizeof(arete));
  int i = 0;

  listeAretes *temp;
  while (aretes) {
    temp = aretes;
    aretes = aretes->suiv;
    g->aretes[i] = temp->a;
    free(temp);
    i++;
  }
}

void freeGrapheCycles(grapheCycles g) {

  free(g.aretes);
  free(g.sommets);
}

void printGrapheCycles(grapheCycles g) {

  int i, j;

  if (g.aretes) {
    for (i = 0; i < g.nb_sommets; i++) {
      printf("s%d :", g.sommets[i].id + 1);

      for (j = 0; j < g.nb_aretes; j++) {
        if (g.aretes[j].id1 == i)
          printf(" s%d", g.aretes[j].id2 + 1);
        else if (g.aretes[j].id2 == i)
          printf(" s%d", g.aretes[j].id1 + 1);
      }
      printf("\n");
    }
  }
}
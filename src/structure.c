#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "structure.h"
#include "utiles.h"
#include "McKay.h"

void procedure(char *nom_dossier, int max_fichiers) {
  
  filenames *fichiers = lireDossier(nom_dossier, max_fichiers);
  if (max_fichiers > 0)
    printFileNames(fichiers);
  
  while (fichiers) {

    graphmol g = lireFichier(nom_dossier, fichiers->nom);

    if (max_fichiers > 0) {
      printGraphMol(g);
      printf("McKay\n");
    }

    grapheCanonique(&g);

    if (max_fichiers > 0)
    printGraphMol(g);

    fichiers = freeFileName(fichiers);
    freeGraphMol(g);
  }
}
 
 // Scanne le dossier nom_dossier et stocke le nom de max_fichiers fichiers dans une liste.
 // Si max_fichiers <= 0, stocke tous les fichiers du dossier (sauf ".", "..").
filenames* lireDossier(char *nom_dossier, int max_fichiers) {
  
  struct dirent *dir;
  DIR *d = opendir(nom_dossier);
  filenames *fichiers = initFileNames();
  int iter = 0;

  if (d) {
    while ((dir = readdir(d)) != NULL && (iter < max_fichiers || max_fichiers <= 0)) {

      size_t taille_allouee = strlen(dir->d_name) ;
      char *nom_fichier = (char *)malloc(taille_allouee);
      strcpy(nom_fichier, dir->d_name);
      if (strcmp(".", nom_fichier) && strcmp("..", nom_fichier)) {
        addFileName(&fichiers, nom_fichier);
        iter++;
      }
    }
    closedir(d);
  }
  printf("Nombre de fichiers : %d\n", iter);
  return fichiers;
}

// Scanne le fichier nom_fichier et stocke le contenu de la matrice d'adjacence dans un graphmol.
// Le nom du fichier doit être le numéro du ChEBI id.
graphmol lireFichier(char* nom_dossier, char *nom_fichier) {
  
  FILE *f;
  graphmol g;
  int nb_sommets, premier, valeur;
  int i, j;

  char path[264];
  sprintf(path, "%s/%s", nom_dossier, nom_fichier);

  f = fopen(path, "r");
  verifScan(fscanf(f, "%d", &nb_sommets), nom_fichier);

  g = initGraphMol(nb_sommets, atoi(nom_fichier));

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

graphmol initGraphMol(int nb_sommets, int chebi_id) {

  graphmol g;
  g.nb_sommets = nb_sommets;
  g.chebi_id = chebi_id;
  g.adjacence = malloc(nb_sommets * sizeof(int*));

  for(int i = 0; i < nb_sommets; i++) {
    g.adjacence[i] = calloc(nb_sommets, sizeof(int));
  }

  return g;
}

void freeGraphMol(graphmol g) {

  for(int i = 0; i < g.nb_sommets; i++) {
    free(g.adjacence[i]);
  }
  free(g.adjacence);
}

void printGraphMol(graphmol g) {
  
  printf("matrice adj de %d : %d\n", g.chebi_id, g.nb_sommets);
  for (int i = 0; i < g.nb_sommets; i++) {
    for (int j = 0; j < g.nb_sommets; j++) {
      printf(" %d", g.adjacence[i][j]);
    }
    printf("\n");
  }
}

filenames* initFileNames(void) {

  filenames *f = malloc(sizeof(struct filenames));
  f->nom = NULL;
  f->suiv = NULL;
  return f;
}

void addFileName(filenames **fichiers, char *nom_fichier) {

  if (fichiers && (*fichiers)->nom == NULL) {
    (*fichiers)->nom = nom_fichier;
  }
  else {
    filenames *nouveau = malloc(sizeof(struct filenames));
    nouveau->nom = nom_fichier;
    nouveau->suiv = *fichiers;
    *fichiers = nouveau;
  }
}

filenames* freeFileName(filenames *fichier) {
  
  if (fichier) {
    filenames *suiv;
    suiv = fichier->suiv;
    free(fichier->nom);
    free(fichier);
    return suiv;
  }
  return NULL;
}

void printFileNames(filenames *fichiers) {
  
  if (fichiers) {
    printf("%s", fichiers->nom);
    filenames *fichier = fichiers->suiv;
    while(fichier) {
      printf(", %s", fichier->nom);
      fichier = fichier->suiv;
    }
    printf("\n");
  }
}
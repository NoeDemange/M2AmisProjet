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
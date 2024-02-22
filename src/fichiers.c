#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fichiers.h"
#include "structure.h"
#include "utiles.h"
#include "McKay.h"
#include "grapheCycles.h"
#include "parcours.h"

void test(char *nom_dossier, char *chebi_id) {

  grapheMol graphe_mol;
  listeCycles *cycles;
  grapheCycles graphe_cycles;
  indexCycles *index_cycles;

  graphe_mol = lireFichier(nom_dossier, chebi_id);
  index_cycles = initIndexCycles(graphe_mol.nb_sommets);
  printGrapheMol(graphe_mol);

  grapheCanonique(&graphe_mol);
  printGrapheMol(graphe_mol);

  cycles = baseDeCyclesMinimale(graphe_mol);
  printListeCycles(cycles);
    
  graphe_cycles = transfoGrapheCycles(graphe_mol, cycles, index_cycles);
  printGrapheCycles(graphe_cycles);

  freeTousListeCycles(cycles);
  freeGrapheMol(graphe_mol);
  freeGrapheCycles(graphe_cycles);
  freeIndexCycles(index_cycles);
}

void procedure(char *nom_dossier, int max_fichiers) {
  
  grapheMol graphe_mol;
  listeCycles *cycles;
  grapheCycles graphe_cycles;
  int max_sommets;

  listeFichiers *fichiers = lireDossier(nom_dossier, max_fichiers, &max_sommets);
  indexCycles *index_cycles = initIndexCycles(max_sommets);

  printf("Max sommets : %d\n", max_sommets);

  if (max_fichiers > 0)
    printListeFichiers(fichiers);
  
  while (fichiers) {

    graphe_mol = lireFichier(nom_dossier, fichiers->nom);

    // TODO tester si g a plus de 3 sommets.

    if (max_fichiers > 0)
      printGrapheMol(graphe_mol);

    grapheCanonique(&graphe_mol);

    if (max_fichiers > 0)
      printGrapheMol(graphe_mol);

    cycles = baseDeCyclesMinimale(graphe_mol);
    if (max_fichiers > 0)
      printListeCycles(cycles);
    
    graphe_cycles = transfoGrapheCycles(graphe_mol, cycles, index_cycles);
    if (max_fichiers > 0)
      printGrapheCycles(graphe_cycles);

    resetIndexCycles(index_cycles, graphe_mol.nb_sommets);
    
    freeTousListeCycles(cycles);
    freeGrapheMol(graphe_mol);
    freeGrapheCycles(graphe_cycles); // Temporaire
    fichiers = freeListeFichiers(fichiers);
  }
  freeIndexCycles(index_cycles);
}
 
 // Scanne le dossier nom_dossier et stocke le nom de max_fichiers fichiers dans une liste.
 // Si max_fichiers <= 0, stocke tous les fichiers du dossier (sauf ".", "..").
listeFichiers* lireDossier(char *nom_dossier, int max_fichiers, int *max_sommets) {
  
  struct dirent *dir;
  DIR *d = opendir(nom_dossier);
  listeFichiers *fichiers = initListeFichiers();
  char *temp;
  int nb_sommets;
  int iter = 0;
  *max_sommets = 0;

  if (d) {
    while ((dir = readdir(d)) != NULL && (iter < max_fichiers || max_fichiers <= 0)) {

      char *nom_fichier = allouerChaine(dir->d_name);

      if (strcmp(".", nom_fichier) && strcmp("..", nom_fichier)) {

        ajouterNomFichier(&fichiers, nom_fichier);
        
        temp = strtok(dir->d_name, "_");
        temp = strtok(NULL, "_");
        nb_sommets = atoi(temp);

        if (*max_sommets < nb_sommets) {
          *max_sommets = nb_sommets;
        }

        iter++;
      }
    }
    closedir(d);
  }
  printf("Nombre de fichiers : %d\n", iter);
  return fichiers;
}

char* allouerChaine(char *chaine) {

  size_t taille_allouee = strlen(chaine) ;
  char *nouveau = (char *)malloc(taille_allouee + 1);
  strcpy(nouveau, chaine);

  return nouveau;
}

char* trouverNomFichier(char *nom_dossier, char *chebi_id) {

  struct dirent *dir;
  DIR *d = opendir(nom_dossier);
  char *debut, *nom_fichier = NULL;
  char *copie;

   if (d) {
    while ((dir = readdir(d)) != NULL) {

      copie = allouerChaine(dir->d_name);
      debut = strtok(dir->d_name, "_");

      if (strcmp(debut, chebi_id) == 0) {
        nom_fichier = copie;
        break;
      }
      free(copie);
    }
    closedir(d);
  }
  if (!nom_fichier) {
    printf("Pas de fichier correspondant à la molécule %s.\n", chebi_id);
    exit(EXIT_FAILURE);
  }
  return nom_fichier;
}

// Scanne le fichier nom_fichier et stocke le contenu de la matrice d'adjacence dans un grapheMol.
// Quand TEST, le nom du fichier doit être le numéro du ChEBI id.
// Sinon, il s'agit du nom complet.
grapheMol lireFichier(char* nom_dossier, char *nom_fichier) {
  
  FILE *f;
  grapheMol g;
  int nb_sommets, premier, valeur;
  char *types = NULL;
  int i, j;
  char path[264];
  char c;

  #ifdef TEST
    char *nom_complet = trouverNomFichier(nom_dossier, nom_fichier);
    sprintf(path, "%s/%s", nom_dossier, nom_complet);
    free(nom_complet);

    f = fopen(path, "r");

    verifScan(fscanf(f, "%d", &nb_sommets), nom_fichier);
  #else
    sprintf(path, "%s/%s", nom_dossier, nom_fichier);

    f = fopen(path, "r");

    verifScan(fscanf(f, "%d", &nb_sommets), strtok(nom_fichier, "_"));
  #endif
  g = initGrapheMol(nb_sommets, atoi(nom_fichier));

  types = malloc(nb_sommets + 1 * sizeof(char));

  for (i = 0; i < nb_sommets; i++) {
    verifScan(fscanf(f, "%d", &premier), nom_fichier);
    for (j = i + 1; j < nb_sommets; j++) {
      verifScan(fscanf(f, " %d", &valeur), nom_fichier);
      g.adjacence[i][j] = valeur;
      g.adjacence[j][i] = valeur;
    }
  }
  verifScan(fscanf(f, "%c", &c), nom_fichier);
  while (c == '\n') {
    verifScan(fscanf(f, "%c", &c), nom_fichier);
  }
  types[0] = c;
  for (i = 1; i < nb_sommets; i++) {
     verifScan(fscanf(f, "%c", &c), nom_fichier);
    types[i] = c;
  }
  types[nb_sommets] = '\0';
  g.types = types;

  fclose(f);

  return g;
}
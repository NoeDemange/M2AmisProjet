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

void test(char *nom_dossier, char *nom_fichier) {

  grapheMol graphe_mol;
  listeCycles *cycles;
  grapheCycles graphe_cycles;
  indexCycles *index_cycles;

  graphe_mol = lireFichier(nom_dossier, nom_fichier);
  index_cycles = initIndexCycles(graphe_mol.nb_sommets);
  printGrapheMol(graphe_mol);

  grapheCanonique(&graphe_mol);
  printGrapheMol(graphe_mol);

  cycles = baseDeCyclesMinimale(graphe_mol);
  printListeCycles(cycles);
    
  graphe_cycles = transfoGrapheCycles(graphe_mol, cycles, index_cycles);
  printGrapheCycles(graphe_cycles);

  freeListeCycles(cycles);
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

    
    freeListeCycles(cycles);
    freeGrapheMol(graphe_mol);
    freeGrapheCycles(graphe_cycles); // Temporaire
    freeIndexCycles(index_cycles);
    fichiers = freeListeFichiers(fichiers);
  }
}
 
 // Scanne le dossier nom_dossier et stocke le nom de max_fichiers fichiers dans une liste.
 // Si max_fichiers <= 0, stocke tous les fichiers du dossier (sauf ".", "..").
listeFichiers* lireDossier(char *nom_dossier, int max_fichiers, int *max_sommets) {
  
  struct dirent *dir;
  DIR *d = opendir(nom_dossier);
  FILE *f;
  listeFichiers *fichiers = initListeFichiers();
  int nb_sommets;
  int iter = 0;
  *max_sommets = 0;

  if (d) {
    while ((dir = readdir(d)) != NULL && (iter < max_fichiers || max_fichiers <= 0)) {

      size_t taille_allouee = strlen(dir->d_name) ;
      char *nom_fichier = (char *)malloc(taille_allouee + 1);
      strcpy(nom_fichier, dir->d_name);

      if (strcmp(".", nom_fichier) && strcmp("..", nom_fichier)) {

        ajouterNomFichier(&fichiers, nom_fichier);
        
        char path[264];
        sprintf(path, "%s/%s", nom_dossier, nom_fichier);
        f = fopen(path, "r");
        verifScan(fscanf(f, "%d", &nb_sommets), nom_fichier);
        fclose(f);

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
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
#include "similarite.h"

void test(char *nom_dossier, char *nom_fichier, char *nom_fichier1) {

  grapheMol graphe_mol;
  listeCycles *cycles;
  grapheCycles graphe_cycles;
  indexCycles *index_cycles;

  graphe_mol = lireFichier(nom_dossier, nom_fichier);
  index_cycles = initIndexCycles(graphe_mol.nb_sommets);
  //printGrapheMol(graphe_mol);

  grapheCanonique(&graphe_mol);
  //printGrapheMol(graphe_mol);

  cycles = baseDeCyclesMinimale(graphe_mol);
  //printListeCycles(cycles);
    
  graphe_cycles = transfoGrapheCycles(graphe_mol, cycles, index_cycles);
  //printGrapheCycles(graphe_cycles);

  generate_dot_file(&graphe_cycles);

  grapheMol graphe_mol1;
  listeCycles *cycles1;
  grapheCycles graphe_cycles1;
  indexCycles *index_cycles1;

  graphe_mol1 = lireFichier(nom_dossier, nom_fichier1);
  index_cycles1 = initIndexCycles(graphe_mol1.nb_sommets);
  // //printGrapheMol(graphe_mol);

  grapheCanonique(&graphe_mol1);
  // //printGrapheMol(graphe_mol);

  cycles1 = baseDeCyclesMinimale(graphe_mol1);
  // //printListeCycles(cycles);
    
  graphe_cycles1 = transfoGrapheCycles(graphe_mol1, cycles1, index_cycles1);

  float sim = similarite(graphe_cycles,graphe_cycles1);
  printf("SIMILARITE : %f\n", sim);

  freeListeCycles(cycles);
  freeGrapheMol(graphe_mol);
  freeGrapheCycles(graphe_cycles);
  freeIndexCycles(index_cycles);
  freeListeCycles(cycles1);
  freeGrapheMol(graphe_mol1);
  freeGrapheCycles(graphe_cycles1);
  freeIndexCycles(index_cycles1);
}

void procedure(char *nom_dossier, int max_fichiers) {

  grapheMol graphe_mol;
  listeCycles *cycles;
  //grapheCycles graphe_cycles;
  int max_sommets;
  int nb_fichiers = 0;

  listeFichiers *fichiers = lireDossier(nom_dossier, max_fichiers, &max_sommets, &nb_fichiers);
  indexCycles *index_cycles = initIndexCycles(max_sommets);

  grapheCycles *liste_GC = malloc(nb_fichiers * sizeof(grapheCycles));

  //printf("Max sommets : %d\n", max_sommets);

  // if (max_fichiers > 0)
  //   printListeFichiers(fichiers);
  
  int pos_fic = 0;
  while (fichiers) {

    graphe_mol = lireFichier(nom_dossier, fichiers->nom);
    //printf("%d",graphe_mol.chebi_id);
    // TODO tester si g a plus de 3 sommets.

    // if (max_fichiers > 0)
    //   printGrapheMol(graphe_mol);

    grapheCanonique(&graphe_mol);

    // if (max_fichiers > 0)
    //   printGrapheMol(graphe_mol);

    cycles = baseDeCyclesMinimale(graphe_mol);
    // if (max_fichiers > 0)
    //   printListeCycles(cycles);
    
    liste_GC[pos_fic] = transfoGrapheCycles(graphe_mol, cycles, index_cycles);
    // if (max_fichiers > 0)
    //   printGrapheCycles(graphe_cycles);

    //generate_dot_file(&graphe_cycles);
    resetIndexCycles(index_cycles, graphe_mol.nb_sommets);
    
    freeListeCycles(cycles);
    freeGrapheMol(graphe_mol);
    //freeGrapheCycles(graphe_cycles); // Temporaire
    fichiers = freeListeFichiers(fichiers);

    pos_fic++;
  }
  freeIndexCycles(index_cycles);

  float **matRes = (float **)malloc((nb_fichiers-1) * sizeof(float*));
  if(matRes==NULL){
    printf("ERROR MATRICE RESULTATS\n");
    exit(666);
  }
  for(int i=0; i<nb_fichiers-1; i++){
    matRes[i] = (float *)malloc((i+1) * sizeof(float));
    if(matRes[i]==NULL){
      printf("ERROR MATRICE RESULTATS\n");
      exit(666);
    }
  }

 
  for(int i = 1; i<nb_fichiers; i++){
    print("Calcul de similarité pour %d : CHEBI:%d",i, liste_GC[i].chebi_id);
    for(int j = 0; j<i; j++){
     float sim = similarite(liste_GC[i], liste_GC[j]);
      matRes[i-1][j] = sim;
      //printf("sim %f; mat %f \n", sim, matRes[i-1][j]);
    }
  }

 writeMatrixToCSV(nb_fichiers,matRes, liste_GC, "matRes.csv");

  for(int i = 0; i<nb_fichiers; i++){
    freeGrapheCycles(liste_GC[i]);
  }
  free(liste_GC);

}
 
 // Scanne le dossier nom_dossier et stocke le nom de max_fichiers fichiers dans une liste.
 // Si max_fichiers <= 0, stocke tous les fichiers du dossier (sauf ".", "..").
listeFichiers* lireDossier(char *nom_dossier, int max_fichiers, int *max_sommets, int *iter) {
  
  struct dirent *dir;
  DIR *d = opendir(nom_dossier);
  FILE *f;
  listeFichiers *fichiers = initListeFichiers();
  int nb_sommets;
  //int iter = 0;
  *max_sommets = 0;

  if (d) {
    while ((dir = readdir(d)) != NULL && ((*iter) < max_fichiers || max_fichiers <= 0)) {

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

        (*iter)++;
      }
    }
    closedir(d);
  }
  printf("Nombre de fichiers : %d\n", (*iter));
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
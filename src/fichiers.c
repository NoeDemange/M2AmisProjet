#include "fichiers.h"

void test(char *nom_dossier, char *chebi_id, char *chebi_id1) {

  grapheMol graphe_mol;
  listeCycles *cycles;
  grapheCycles graphe_cycles;
  indexCycles *index_cycles;

  graphe_mol = lireFichier(nom_dossier, chebi_id);
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

  graphe_mol1 = lireFichier(nom_dossier, chebi_id1);
  index_cycles1 = initIndexCycles(graphe_mol1.nb_sommets);
  // //printGrapheMol(graphe_mol);

  grapheCanonique(&graphe_mol1);
  // //printGrapheMol(graphe_mol);

  cycles1 = baseDeCyclesMinimale(graphe_mol1);
  // //printListeCycles(cycles);
    
  graphe_cycles1 = transfoGrapheCycles(graphe_mol1, cycles1, index_cycles1);

  float sim = similarite(graphe_cycles,graphe_cycles1);
  printf("SIMILARITE : %f\n", sim);
  
  freeGrapheMol(graphe_mol);
  freeGrapheCycles(graphe_cycles);
  freeIndexCycles(index_cycles);
  freeTousListeCycles(cycles);
  freeTousListeCycles(cycles1);
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
    
    freeTousListeCycles(cycles);
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
    printf("Calcul de similarité pour %d : CHEBI:%d\n",i, liste_GC[i].chebi_id);
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
  listeFichiers *fichiers = initListeFichiers();
  char *temp;
  int nb_sommets;
  //int iter = 0;
  *max_sommets = 0;

  if (d) {
    while ((dir = readdir(d)) != NULL && ((*iter) < max_fichiers || max_fichiers <= 0)) {

      char *nom_fichier = allouerChaine(dir->d_name);

      if (strcmp(".", nom_fichier) && strcmp("..", nom_fichier)) {

        ajouterNomFichier(&fichiers, nom_fichier);
        
        temp = strtok(dir->d_name, "_");
        temp = strtok(NULL, "_");
        nb_sommets = atoi(temp);

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
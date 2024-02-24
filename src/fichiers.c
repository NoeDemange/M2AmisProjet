#include "fichiers.h"

void comparaison(char *nom_dossier, char *chebi_id, char *chebi_id1, int dot_option) {

  grapheMol graphe_mol = lireFichier(nom_dossier, chebi_id, 1);
  grapheMol graphe_mol1 = lireFichier(nom_dossier, chebi_id1, 1);

  indexCycles *index_cycles = initIndexCycles(MAX(graphe_mol.nb_sommets,
                                                  graphe_mol1.nb_sommets));

  grapheCycles graphe_cycles = genererGrapheCycles(graphe_mol, index_cycles);
  grapheCycles graphe_cycles1 = genererGrapheCycles(graphe_mol1, index_cycles);
 
 if (dot_option) {
    genererFichierDotGM(&graphe_mol);
    genererFichierDotGM(&graphe_mol1);
    genererFichierDotGC(&graphe_cycles);
    genererFichierDotGC(&graphe_cycles1);
  }

  freeGrapheMol(graphe_mol);
  freeGrapheMol(graphe_mol1);
  freeIndexCycles(index_cycles);

  float sim = similarite(graphe_cycles,graphe_cycles1,dot_option);
  float lev = distLevenshteinNormalise(graphe_cycles, graphe_cycles1, NULL);
  printf("Similarité : %f\n", sim * lev);
  
  freeGrapheCycles(graphe_cycles);
  freeGrapheCycles(graphe_cycles1);
}

void procedure(char *nom_dossier, int max_fichiers, char *chebi_id) {

  grapheMol graphe_mol;
  listeCycles *cycles;
  int max_sommets, pos_fic;
  int nb_fichiers = 0;
  char *nom_ref = NULL;

  listeFichiers *fichiers = lireDossier(nom_dossier, max_fichiers, &max_sommets, &nb_fichiers);
  indexCycles *index_cycles;
  grapheCycles *liste_GC = NULL;

  if (chebi_id) {
    nom_ref = trouverNomFichier(nom_dossier, chebi_id);
    graphe_mol = lireFichier(nom_dossier, chebi_id, 1);
    liste_GC = allouer((nb_fichiers+1) * sizeof(grapheCycles), "liste des graphes de cycles (fichiers.c)");
    max_sommets = MAX(graphe_mol.nb_sommets,max_sommets);
    index_cycles = initIndexCycles(max_sommets);
    liste_GC[0] = genererGrapheCycles(graphe_mol, index_cycles);
    freeGrapheMol(graphe_mol);
    pos_fic = 1;
  }
  else {
    liste_GC = allouer(nb_fichiers * sizeof(grapheCycles), "liste des graphes de cycles (fichiers.c)");
    index_cycles = initIndexCycles(max_sommets);
    pos_fic = 0;
  }
  while (fichiers) {

    if(nom_ref && strcmp(nom_ref, fichiers->nom) == 0) {
      fichiers = freeListeFichiers(fichiers);
      continue;
    }

    graphe_mol = lireFichier(nom_dossier, fichiers->nom, 0);
    
    if (graphe_mol.nb_sommets < 3) {
      freeGrapheMol(graphe_mol);
      fichiers = freeListeFichiers(fichiers);
      continue;
    }
    grapheCanonique(&graphe_mol);

    cycles = baseDeCyclesMinimale(graphe_mol);
    liste_GC[pos_fic] = transfoGrapheCycles(graphe_mol, cycles, index_cycles);

    freeGrapheMol(graphe_mol); 
    fichiers = freeListeFichiers(fichiers);
    pos_fic++;
  }
  if (nom_ref) {
    free(nom_ref);
  }
  freeIndexCycles(index_cycles);

  float sim, lev;
  char *fichier_sortie = "matRes.csv";
  int **bufferDist = allouer(2 * sizeof(int*), "matrice de distance de Levenshtein (fichier.c)");
  for (int i = 0; i < 2; i++) {
    bufferDist[i] = allouer((max_sommets + 1) * sizeof(int), "matrice de distance de Levenshtein (fichier.c)");
  }
  if (chebi_id) {
    float *resultats = allouer((pos_fic) * sizeof(float), "tableau de similarité (fichiers.c)");
    
    
    for (int i = 1; i < pos_fic; i++) {
      printf("Calcul des similarités pour %d : CHEBI:%d\n",i, liste_GC[i].chebi_id);
      sim = similarite(liste_GC[0], liste_GC[i],0);
      lev = distLevenshteinNormalise(liste_GC[0], liste_GC[i], bufferDist);
      resultats[i] = sim * lev;
    }
    printf("Ecriture du tableau de similarité dans %s.\n", fichier_sortie);
    ecrireTableauDansCSV(pos_fic, resultats, liste_GC, fichier_sortie);

    free(resultats);
  }
  else {
    float **resultats = allouer((nb_fichiers - 1) * sizeof(float*), "matrice de similarité (fichiers.c)");
    for (int i = 0; i < nb_fichiers - 1; i++) {
      resultats[i] = allouer((i + 1) * sizeof(float), "matrice de similarité (fichier.c)");
    }
    
    for (int i = 0; i < nb_fichiers; i++) {
      printf("Calcul des similarités pour %d : CHEBI:%d\n",i, liste_GC[i].chebi_id);
      for (int j = i + 1; j < nb_fichiers; j++) {
        sim = similarite(liste_GC[i], liste_GC[j],0);
        lev = distLevenshteinNormalise(liste_GC[i], liste_GC[j], bufferDist);
        resultats[j - 1][i] = sim * lev;
      }
    }
    
    printf("Ecriture de la matrice de similarité dans %s.\n", fichier_sortie);
    ecrireMatriceDansCSV(nb_fichiers, resultats, liste_GC, fichier_sortie);

    for (int i = 1; i < nb_fichiers; i++) {
      free(resultats[i - 1]);
    }
    free(resultats);
  }
  free(bufferDist[0]);
  free(bufferDist[1]);
  free(bufferDist);
  for (int i = 0; i < nb_fichiers; i++) {
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
      else {
        free(nom_fichier);
      }
    }
    closedir(d);
  }
  printf("Nombre de fichiers dans %s : %d\n", nom_dossier, (*iter));
  return fichiers;
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
    exit(1);
  }
  return nom_fichier;
}

// Scanne le fichier nom_fichier et stocke le contenu de la matrice d'adjacence dans un grapheMol.
// Quand TEST, le nom du fichier doit être le numéro du ChEBI id.
// Sinon, il s'agit du nom complet.
grapheMol lireFichier(char* nom_dossier, char *nom_fichier, int opt) {
  
  FILE *f;
  grapheMol g;
  int nb_sommets, premier, valeur;
  char **types = NULL;
  int i, j;
  char chemin[264];
  char c;

  if (opt) {
    char *nom_complet = trouverNomFichier(nom_dossier, nom_fichier);
    sprintf(chemin, "%s/%s", nom_dossier, nom_complet);
    free(nom_complet);
    f = ouvrirFichier(chemin, "r");
    scannerFichier(fscanf(f, "%d", &nb_sommets), nom_fichier);
  }
  else {
    sprintf(chemin, "%s/%s", nom_dossier, nom_fichier);
    f = ouvrirFichier(chemin, "r");
    scannerFichier(fscanf(f, "%d", &nb_sommets), strtok(nom_fichier, "_"));
  }

  g = initGrapheMol(nb_sommets, atoi(nom_fichier));

  types = allouer(nb_sommets * sizeof(char*), "chaînes de caractères des types d'atomes (fichiers.c)");

  for (i = 0; i < nb_sommets; i++) {
    scannerFichier(fscanf(f, "%d", &premier), nom_fichier);
    for (j = i + 1; j < nb_sommets; j++) {
      scannerFichier(fscanf(f, " %d", &valeur), nom_fichier);
      g.adjacence[i][j] = valeur;
      g.adjacence[j][i] = valeur;
    }
  }
  i = 0;
  int pos_deb = 0, pos_fin;
  while(!feof(f)) {
    c = fgetc(f);
    if (c == 'H') {
      c = fgetc(f);
      if (c == ' ') {
        printf("[INFO] Attention, atome H encore présent dans ChEBI:%s.\n", nom_fichier);
      }
      // Part du principe qu'il n'y a pas de type commençant 
      // par H suivit de plus d'une lettre
      else {
        pos_deb = ftell(f) - 2;
      }
    }
    else if (c > 'A' && c < 'Z') {
      pos_deb = ftell(f) - 1;
    }
    else if (c == ' ') {
      pos_fin = ftell(f) - 1;
      types[i] = allouer((pos_fin - pos_deb + 1) * sizeof(char), "chaînes de caractères des types d'atomes (fichiers.c)");
      fseek(f, pos_deb, SEEK_SET);
      for (j = 0; j < pos_fin - pos_deb; j++) {
        c = fgetc(f);
        types[i][j] = c;
      }
      types[i][pos_fin - pos_deb] = '\0';
      c = fgetc(f);
      i++;
    }
  }
  g.types = types;

  fclose(f);

  return g;
}

//Generer Dot pour grapheCycles
void genererFichierDotGC(grapheCycles *g) {
    
    char nom_dossier[100];
    sprintf(nom_dossier, "graphs");
    creerDossier(nom_dossier);

    char nom_fichier[100];
    sprintf(nom_fichier, "graphs/GC%d.dot", g->chebi_id);

    FILE *fp = ouvrirFichier(nom_fichier, "w");
    if (fp == NULL) {
        printf("Error opening file.\n");
        return;
    }

    // Header du fichier DOT
    fprintf(fp, "graph G {\n");

    // Sommets
    for (int i = 0; i < g->nb_sommets; i++) {
        fprintf(fp, "    %d [label=\"%d, %d\", shape=circle];\n", g->sommets[i].id, g->sommets[i].id, g->sommets[i].taille);
    }

    // Arêtes
    char *couleur;
    for (int i = 0; i < g->nb_sommets-1; i++) {
        for (int j = i + 1; j < g->nb_sommets; j++) {
            if (g->types_aretes[i][j].type != AUCUNE_LIAISON) {
                couleur = (g->types_aretes[i][j].type == 1) ? "blue" : "green";
                fprintf(fp, "    %d -- %d [label=\"%d\", color=%s];\n", i, j, g->types_aretes[i][j].poids, couleur);
            }
        }
    }

    fprintf(fp, "}\n");

    fclose(fp);
}

//Generer Dot pour grapheMoléculaires
void genererFichierDotGM(grapheMol *g) {
    
    char nom_dossier[100];
    sprintf(nom_dossier, "graphs");
    creerDossier(nom_dossier);

    char nom_fichier[100];
    sprintf(nom_fichier, "graphs/GM%d.dot", g->chebi_id);

    FILE *fp = ouvrirFichier(nom_fichier, "w");
    if (fp == NULL) {
        printf("Error opening file.\n");
        return;
    }

    // Header du fichier DOT
    fprintf(fp, "graph G {\n");

    // Sommets
    char *couleur_s;
    for (int i = 0; i < g->nb_sommets; i++) {
        if (strcmp(g->types[i], "C") == 0)
            couleur_s = "black";
        else if (strcmp(g->types[i], "O") == 0)
            couleur_s = "red";
        else if (strcmp(g->types[i], "N") == 0)
            couleur_s = "blue";
        else if (strcmp(g->types[i], "F") == 0)
            couleur_s = "chartreuse2";
        else if (strcmp(g->types[i], "P") == 0)
            couleur_s = "blueviolet";
        else if (strcmp(g->types[i], "S") == 0)
            couleur_s = "yellow";
        else if (strcmp(g->types[i], "Cl") == 0)
            couleur_s = "chartreuse4";
        else if (strcmp(g->types[i], "Fe") == 0 || strcmp(g->types[i], "Co") == 0 ||
                 strcmp(g->types[i], "Ni") == 0 || strcmp(g->types[i], "Cu") == 0)
            couleur_s = "azure4";
        else if (strcmp(g->types[i], "Br") == 0)
            couleur_s = "darkgreen";
        else if (strcmp(g->types[i], "I") == 0)
            couleur_s = "springgreen4";
        else
            couleur_s = "burlywood4";
        fprintf(fp, "    %d [label=\"%d,%s\", shape=circle, color=%s];\n", i, i, g->types[i], couleur_s);
    }

    // Arêtes
    for (int i = 0; i < g->nb_sommets-1; i++) {
        for (int j = i + 1; j < g->nb_sommets; j++) {
            if (g->adjacence[i][j] != 0) { // Si le poids de l'arête est différent de zéro
                fprintf(fp, "    %d -- %d ;\n", i, j);
            }
        }
    }

    fprintf(fp, "}\n");

    fclose(fp);
}

//Generer Dot pour grapheSim
void genererFichierDotGP(grapheSim *g, couple* sommets, int* clique, int id1, int id2) {
    
    char nom_dossier[100];
    sprintf(nom_dossier, "graphs");
    creerDossier(nom_dossier);

    char nom_fichier[100];
    sprintf(nom_fichier, "graphs/GP%d-%d.dot", id1, id2);

    FILE *fp = ouvrirFichier(nom_fichier, "w");
    if (fp == NULL) {
        printf("Error opening file.\n");
        return;
    }

    // Header du fichier DOT
    fprintf(fp, "graph G {\n");

    // Sommets
    char* couleur_s;
    for (int i = 0; i < g->nb_sommets; i++) {
      couleur_s = (clique[i] == 1) ? "red" : "black";
        fprintf(fp, "    %d [label=\"(%d,%d)\", shape=circle, color = %s];\n", i, sommets[i].id1, sommets[i].id2, couleur_s);
    }

    // Arêtes
    char* couleur;
    for (int i = 0; i < g->nb_sommets-1; i++) {
        for (int j = i + 1; j < g->nb_sommets; j++) {
            if (g->adjacence[i][j] != 0) { // Si le poids de l'arête est différent de zéro
                couleur = "black";
                if(clique[i]==1 && clique[j]==1) {
                  couleur = "red";
                }
                fprintf(fp, "    %d -- %d [color = %s];\n", i, j, couleur);
            }
        }
    }

    fprintf(fp, "}\n");

    fclose(fp);
}

// Écrit la matrice dans un fichier CSV
void ecrireMatriceDansCSV(int n, float **matrice, grapheCycles *liste_GC, const char *nom_fichier) {
    
    FILE* fp = ouvrirFichier(nom_fichier, "w");

    fprintf(fp, ";");

    for (int i = 0; i < n; i++) {
        // Écrire nom fichier
        fprintf(fp, "%d", liste_GC[i].chebi_id);
            if (i < n - 1) {
                fprintf(fp, ";");
            }
    }
    fprintf(fp, "\n");

    // Écrire la matrice dans le fichier CSV
    for (int i = 0; i < n; i++) {
        fprintf(fp, "%d;", liste_GC[i].chebi_id);
        for (int j = 0; j < n; j++) {
            if (i == j)
                fprintf(fp, "%f", 1.0);
            else if (i < j)
                fprintf(fp, "%f", matrice[j-1][i]);
            else
                fprintf(fp, "%f", matrice[i-1][j]);
            if (j < n - 1)
                fprintf(fp, ";");
        }
        fprintf(fp, "\n");
    }
    fclose(fp);
}

void ecrireTableauDansCSV(int n, float *tableau, grapheCycles *liste_GC, const char *nom_fichier) {
    
    FILE* fp = ouvrirFichier(nom_fichier, "w");

    fprintf(fp, ";");
    
    // Écrire nom référence
    fprintf(fp, "%d", liste_GC[0].chebi_id);
    fprintf(fp, "\n");

    // Écrire le tableau dans le fichier CSV
    for (int i = 0; i < n; i++) {
        fprintf(fp, "%d;", liste_GC[i].chebi_id);
        if (i == 0)
            fprintf(fp, "%f", 1.0);
        else
            fprintf(fp, "%f", tableau[i]);
        fprintf(fp, "\n");
    }
    fclose(fp);
}


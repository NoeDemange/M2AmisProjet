#include "utiles.h"



double chrono() {
  
	struct timeval tv;
	static double date_deb = 0.0;
	double date_courante;
	gettimeofday(&tv, NULL);
	date_courante = tv.tv_sec + ((double) tv.tv_usec) * 1e-6;
	if (date_deb == 0.0) {
        date_deb = date_courante;
    }
	return date_courante - date_deb;
}

void tempsExecution(double sec, char *info) {

  int h, m;
  double s;
  h = (sec/3600); 
  m = (sec -(3600*h))/60;
  s = (sec -(3600*h)-(m*60));
	
  printf("%s - %dh:%dm:%fs\n",info, h, m, s);	
}

void verifScan(int valeur, char *chebi_id) {
  
  if (valeur != 1) {
    printf("Erreur de lecture du fichier pour la molécule %s.\n", chebi_id);
    exit(EXIT_FAILURE);
  }
}

int absolue(int a) {

	return ( a > 0) ? a : -a;
}

int min( int a , int b) {
	
	return (a < b) ? a : b;
}

int minTrois(int a, int b, int c) {

  return a < b ? (a < c ? a : c) : (b < c ? b : c);
}

void printTab(int *tab, int n) {
    
    int i;
    for (i = 0; i < n; i++)
        printf("%d ", tab[i]);
    printf("\n");
}

void printMatrice(int **matrice, int n, int m) {

  int i, j;
  for (i = 0; i < n; i++) {
    for (j = 0; j < m; j++) {
      printf(" %d", matrice[i][j]);
    }
    printf("\n");
  }
  printf("\n");
}

void creerDossier(const char *path) {
    
    #ifdef _WIN32
        mkdir(path);
    #else
        mkdir(path, 0777);
    #endif
}

void genererFichierDot(grapheCycles *g) {
    
    char nom_dossier[100];
    sprintf(nom_dossier, "graphs");
    creerDossier(nom_dossier);

    char nom_fichier[100];
    sprintf(nom_fichier, "graphs/%d.dot", g->chebi_id);

    FILE *fp = fopen(nom_fichier, "w");
    if (fp == NULL) {
        printf("Error opening file.\n");
        return;
    }

    // Write the header for the DOT file
    fprintf(fp, "graph G {\n");

    // Write vertices
    for (int i = 0; i < g->nb_sommets; i++) {
        fprintf(fp, "    %d [label=\"%d\", shape=circle];\n", g->sommets[i].id, g->sommets[i].taille);
    }

    // Write edges
    char *couleur;
    for (int i = 0; i < g->nb_sommets; i++) {
        for (int j = i + 1; j < g->nb_sommets; j++) {
            if (g->types_aretes[i][j].type != AUCUNE_LIAISON) {
                couleur = (g->types_aretes[i][j].type == 1) ? "blue" : "green";
                fprintf(fp, "    %d -- %d [label=\"%d\", color=%s];\n", i, j, g->types_aretes[i][j].poids, couleur);
            }
        }
    }

    // Write the footer for the DOT file
    fprintf(fp, "}\n");

    fclose(fp);
}

// Fonction pour écrire la matrice dans un fichier CSV
void ecrireMatriceDansCSV(int n, float **matrice, grapheCycles *liste_GC, const char *nom_fichier) {
    
    FILE* fp = fopen(nom_fichier, "w");

    if (fp == NULL) {
        printf("Erreur lors de l'ouverture du fichier.");
        return;
    }

    fprintf(fp, ";");

    for (int i = 0; i < n; i++) {//écrire nom fichier
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
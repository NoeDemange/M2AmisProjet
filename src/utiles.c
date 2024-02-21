#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/stat.h>
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

void verifScan(int valeur, char *nom_fichier) {
  
  if (valeur != 1) {
    printf("Erreur à la lecture du fichier %s\n", nom_fichier);
    exit(EXIT_FAILURE);
  }
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

void createFolder(const char *path) {
    #ifdef _WIN32
        mkdir(path);
    #else
        mkdir(path, 0777);
    #endif
}

void generate_dot_file(grapheCycles *graph) {
    char foldername[100];
    sprintf(foldername, "graphs");
    createFolder(foldername);

    char filename[100];
    sprintf(filename, "graphs/%d.dot", graph->chebi_id);

    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        printf("Error opening file.\n");
        return;
    }

    // Write the header for the DOT file
    fprintf(fp, "graph G {\n");

    // Write vertices
    for (int i = 0; i < graph->nb_sommets; i++) {
        fprintf(fp, "    %d [label=\"%d\"];\n", graph->sommets[i].id, graph->sommets[i].taille);
    }

    // Write edges
    for (int i = 0; i < graph->nb_aretes; i++) {
        fprintf(fp, "    %d -- %d [label=\"%d\"];\n", graph->aretes[i].id1, graph->aretes[i].id2, graph->aretes[i].poids);
    }

    // Write the footer for the DOT file
    fprintf(fp, "}\n");

    fclose(fp);
}
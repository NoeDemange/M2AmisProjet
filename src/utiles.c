#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>

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

void tempsExecution(double sec) {

  int h, m;
  double s;
  h = (sec/3600); 
  m = (sec -(3600*h))/60;
  s = (sec -(3600*h)-(m*60));
	
  printf("Temps d'exécution - %dh:%dm:%fs\n",h,m,s);	
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
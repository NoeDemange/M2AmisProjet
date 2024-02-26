#include "clique.h"

long chrono_clique() {

	struct timeval tv;
	long date_courante;
	gettimeofday(&tv,NULL);
	date_courante = tv.tv_sec;
	return date_courante;
}

// Détermine récursivement une clique max de g.
// Adapté de l'algorithme de Bron-Kerbosch.
void calculCliqueMax(grapheSim *g, int* dans_clique_max, int* taille_clique_max, int *dans_clique, int taille_clique, int *candidat, int taille_candidat, long date_max) {	
	
  int nb_sommets = g->nb_sommets;
  
	if (date_max != 0) {	
		if(chrono_clique()  > date_max) {
			return;
		}
	}
	int i,j;

	if( taille_candidat == 0) {
		if(taille_clique > *taille_clique_max) {
			*taille_clique_max = taille_clique;
			for (i = 0 ;  i < nb_sommets ; i ++) {
				dans_clique_max[i] = dans_clique[i];
			}
		}
		return;
	}
	if (taille_candidat + taille_clique <= *taille_clique_max) {
		return;
	}
	
	int taille_candidat_temp;
	int *candidat_temp;
	candidat_temp = malloc(nb_sommets * sizeof(int));

	for (i = 0 ;  i < nb_sommets ; i ++) {
		if ( candidat[i] == 1) {
			candidat[i] = 0;
			taille_candidat--;
			dans_clique[i] = 1;
			taille_candidat_temp = taille_candidat;
			
			for (j = 0 ;  j < nb_sommets ; j ++) {
				candidat_temp[j] = candidat[j]; 
				if ((candidat[j] == 1) && (g->adjacence[i][j] == 0)) {
					candidat_temp[j] = 0;
					taille_candidat_temp--;	
				}	
			}
			calculCliqueMax(g,dans_clique_max, taille_clique_max, dans_clique, taille_clique + 1, candidat_temp, taille_candidat_temp, date_max);
			dans_clique[i] = 0;
		}	
	}
	free(candidat_temp);
}

int* cliqueMax(grapheSim *g, long timeout) {

	int i;
  int taille =g->nb_sommets;
  int taille_clique_max = 0;

	int *clique_max = allouer(taille * sizeof(int), "clique max (clique.c)");
  int *X = allouer(taille *sizeof(int), "dans clique X (clique.c)");
	int *P = allouer( taille *sizeof(int), "candidat P (clique.c)");
	
	for (i = 0; i < taille ; i++) {
		P[i] 	= 1;
		X[i]	= 0;
		clique_max[i] = 0;
	}
	
	calculCliqueMax(g, clique_max, &taille_clique_max, X, 0, P, taille, chrono_clique() + timeout);
   
	free(X);
	free(P);
  return clique_max;
}
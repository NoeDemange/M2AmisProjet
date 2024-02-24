#include "clique.h"

long chrono_clique() {

	struct timeval tv;
	long date_courante;
	gettimeofday(&tv,NULL);
	date_courante = tv.tv_sec;
	return date_courante;
}

// Calcul récursif de la clique max
void calcul_cl(grapheSim *g, int* dans_clique_maxR, int* taille_clique_maxR, int *dans_cliqueX,int taille_cliqueX,int *candidatP,int taille_candidatP, long maxdate) {	
	
  int nb_sommets = g->nb_sommets;
  
	if (maxdate != 0) {	
		if(chrono_clique()  > maxdate) {
			return;
		}
	}
	int i,j;

	// printf("\nR(%d) P(%d) X(%d)\n", *taille_clique_maxR, taille_candidatP, taille_cliqueX);
	// printf("R : ");
	// for (i = 0; i < nb_sommets; i++) {
	// 	printf("%d ", dans_clique_maxR[i]);
	// }
	// printf("\nP : ");
	// for (i = 0; i < nb_sommets; i++) {
	// 	printf("%d ", candidatP[i]);
	// }
	// printf("\nX : ");
	// for (i = 0; i < nb_sommets; i++) {
	// 	printf("%d ", dans_cliqueX[i]);
	// }

	if( taille_candidatP == 0) {
		if(taille_cliqueX > *taille_clique_maxR) {
			*taille_clique_maxR = taille_cliqueX;
			for (i = 0 ;  i < nb_sommets ; i ++) {
				dans_clique_maxR[i] = dans_cliqueX[i];
			}
		}
		return;
	}
	if (taille_candidatP + taille_cliqueX <= *taille_clique_maxR) {
		return;
	}
	
	int taille_candidat_temp;
	int *candidat_temp;
	candidat_temp = malloc(nb_sommets * sizeof(int));

	for (i = 0 ;  i < nb_sommets ; i ++) {
		if ( candidatP[i] == 1) {
			candidatP[i] = 0;
			taille_candidatP--;
			dans_cliqueX[i] = 1;
			taille_candidat_temp = taille_candidatP;
			
			for (j = 0 ;  j < nb_sommets ; j ++) {
				candidat_temp[j] = candidatP[j]; 
				if ((candidatP[j] == 1) && (g->adjacence[i][j] == 0)) {
					candidat_temp[j] = 0;
					taille_candidat_temp--;	
				}	
			}
			//printf("\ntaille P: %d\n", taille_candidat_temp);
			calcul_cl(g,dans_clique_maxR,taille_clique_maxR,dans_cliqueX,taille_cliqueX + 1,candidat_temp,taille_candidat_temp,maxdate);
			dans_cliqueX[i] = 0;
		}	
	}
	free(candidat_temp);
}

int* cliqueMax(grapheSim *g, long timeout) {

	// Initialisation
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

	// printMatrice(g->adjacence, g->nb_sommets, g->nb_sommets);

	// printf("\nclique : ");
	// for (i = 0; i < taille; i++) {
	// 	printf("%d ", clique_max[i]);
	// }
	
	calcul_cl(g,clique_max, &taille_clique_max, X, 0, P, taille, chrono_clique() + timeout);
   
	free(X);
	free(P);
  return clique_max;
}
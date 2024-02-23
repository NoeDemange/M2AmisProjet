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
		if(chrono_clique()  > maxdate) 
			return;
	}
	int i,j;

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
			dans_cliqueX[i] = 1 ;
			taille_candidat_temp = taille_candidatP;
			
			for (j = 0 ;  j < nb_sommets ; j ++) {
				candidat_temp[j] = candidatP[j]; 
				if ((candidatP[j] == 1) && (g->adjacence[i][j] == 0)) {
					candidat_temp[j] = 0;
					taille_candidat_temp--;	
				}	
			}
			
			taille_candidat_temp --;
			calcul_cl(g,dans_clique_maxR,taille_clique_maxR,dans_cliqueX,taille_cliqueX + 1,candidat_temp,taille_candidat_temp,maxdate);
			dans_cliqueX[i] = 0;
		}	
	}
	free(candidat_temp);
}
int* cliqueMax(grapheSim *g, long timeout) {

	// Initialisation
	int i;
	int *candidatP;
	int *dans_cliqueX;
  int taille_clique_max;
	int *dans_clique_maxR;

  int taille =g->nb_sommets;
	
  taille_clique_max = 0;

  dans_clique_maxR = malloc(taille * sizeof(int));
	if (!dans_clique_maxR) {
		fprintf(stderr,"cannot malloc dans_clique_maxR %d\n",taille); exit(41); 
	}
  dans_cliqueX = malloc(taille *sizeof(int));
	if (!dans_cliqueX) {
		fprintf(stderr,"cannot malloc dans_cliqueX %d\n",taille); exit(42);
	}
	candidatP = malloc( taille *sizeof(int));
	if (!candidatP) {
		fprintf(stderr,"cannot malloc candidatP %d\n",taille); exit(43);
	}
	
	for (i = 0; i < taille ; i++) {
		candidatP[i] 	= 1;
		dans_cliqueX[i]	= 0;
		dans_clique_maxR[i] = 0;
	}
	
	// 0 taille de la clique initial  et m.nb_atome = nb sommets candidats
	calcul_cl(g,dans_clique_maxR,&taille_clique_max,dans_cliqueX,0,candidatP,taille,chrono_clique() + timeout);
   
	free(dans_cliqueX);
	free(candidatP);
  return dans_clique_maxR;
}
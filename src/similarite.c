#include "similarite.h"

float restrictionTailleCycles(int taille1, int taille2) {

	return (float)(absolue(taille1 - taille2)) <= 0.2 * min(taille1, taille2);
}

// Construction des couples de cycles compatibles
couple* couplesCyclesCompatibles(grapheCycles g1, grapheCycles g2, int *taille) {
	
	int i, j;
	int n;
	couple *couples;

	if (*taille == 0) {
		for(i= 0; i < g1.nb_sommets; i++) {
			for(j= 0; j < g2.nb_sommets; j++) {
				if(restrictionTailleCycles(g1.sommets[i].taille, g2.sommets[j].taille)) {
					(*taille)++;
				}
			}
		}
		if (*taille == 0) {
			return NULL;
		}
	}
	couples = malloc(*taille * sizeof(couple));

	n = 0;
	for (i = 0; i < g1.nb_sommets; i++) { 
		for (j = 0; j < g2.nb_sommets; j++) {
			if(restrictionTailleCycles(g1.sommets[i].taille, g2.sommets[j].taille)) {
				couples[n].id1 = i;
				couples[n].id2 = j;
				n++;
			}
		}
	}
	return couples;
}

grapheSim* produitGraphesCycles(grapheCycles g1, grapheCycles g2) { 

	int taille = 0; // Taille du graphe produit
	int i, j;
	
	// Arêtes entre les cycles "compatibles"
	couple *couples = couplesCyclesCompatibles(g1, g2, &taille);

	if (taille == 0) {
		return NULL;
	}

  int** adjacence =  malloc(taille * sizeof(int*));	
	for(i = 0 ;i < taille; i++) {
		adjacence[i] =  calloc(taille, sizeof(int));
	}
	
	int i1, i2, j1, j2;

	for (i = 0; i < taille; i++) { 
		adjacence[i][i] = 1;

		i1 = couples[i].id1;
		i2 = couples[i].id2;

		for (j = i + 1; j < taille; j++) {
			j1=couples[j].id1;
			j2=couples[j].id2;
			
			if (g1.types_aretes[i1][j1].type == g2.types_aretes[i2][j2].type && g1.types_aretes[i1][j1].poids == g2.types_aretes[i2][j2].poids) {
				if (((i1 == j1) && (g2.types_aretes[i2][j2].type != AUCUNE_LIAISON)) || 
						((i2 == j2) && (g1.types_aretes[i1][j1].type != AUCUNE_LIAISON)) || 
						((i1 != j1) && (i2 != j2)) ||
						((i1 == j1) && (i2 == j2))) {
					adjacence[i][j] = 1;
					adjacence[j][i] = 1;					
				}
			}
		}
	}
	free(couples);
	
	grapheSim *g  = malloc(sizeof(struct grapheSim));
  g->nb_sommets = taille;
  g->adjacence = adjacence;

	return g;
}

// Contruit le graphe commun
int*  grapheCommunG12(int taille, int* clique_max, grapheCycles g1, grapheCycles g2) {

	int* taille_graphe_commun = (int*) malloc(sizeof(int)*2);
  int nb_at =0,nb_liaisons=0,i,j,i1,j1;
	
	couple *couples = couplesCyclesCompatibles(g1, g2, &taille);
	
	int tab[taille];

	for (i = 0; i < taille ; i++) {
		tab[i] = clique_max[i];
	}
	for (i = 0; i < taille - 1; i++) {
		if (tab[i] == 1) {
			for (j = i + 1; j < taille ; j++) {
				if (tab[j] == 1 && (couples[i].id1 == couples[j].id1)) {
					tab[j] = 0;
				}
			}
		}
	}
	for (i = 0; i < taille ; i++) {
		if (tab[i] == 1) {
			nb_at++;
		}
	}
	for (i = 0; i < taille - 1; i++) {
		if (tab[i] == 1) {
			for (j = i + 1; j < taille; j++) {
				if (tab[j] == 1) {
					i1 = couples[i].id1;
					j1 = couples[j].id1;
					if(g1.types_aretes[i1][j1].type != AUCUNE_LIAISON) {
						nb_liaisons ++;
					}
				}
			}
		}
	}
	free(couples);
	taille_graphe_commun[0] = nb_at;
	taille_graphe_commun[1] = nb_liaisons;

	return taille_graphe_commun;
}

float similarite(grapheCycles g1, grapheCycles g2) {

	float sim = 0.0;

	grapheSim *graphe_produit = produitGraphesCycles(g1, g2);

	int taille = graphe_produit->nb_sommets;

	if (taille == 0) {
		return 0;
	}

	int* clique = cliqueMax(graphe_produit, (long)date);

	int* taille_graphe_commun = grapheCommunG12(taille, clique, g1, g2);

	int nb_atomes_communs = taille_graphe_commun[0];	
	int nb_laisons_communs = taille_graphe_commun[1];


	float num = (float)((nb_atomes_communs + nb_laisons_communs) * (nb_atomes_communs + nb_laisons_communs));
	float denum = (float)((g1.nb_sommets + g1.nb_aretes) * (g2.nb_aretes + g2.nb_sommets));

	sim = num/denum;

	free(taille_graphe_commun);
	free(clique);

	for (int i = 0; i < graphe_produit->nb_sommets; i++) {
		free(graphe_produit->adjacence[i]);
	}
	free(graphe_produit->adjacence);
	free(graphe_produit);

	return sim;
}
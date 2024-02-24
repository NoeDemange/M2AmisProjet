#include "similarite.h"

float similarite(grapheCycles g1, grapheCycles g2, int dot_option) {

	float sim = 0.0;

	int taille;

	// Ensemble des sommets du graphe produit formé par 
	// les cycles "compatibles" des deux graphes
	couple *sommets = couplesCyclesCompatibles(g1, g2, &taille);

	if (taille == 0) {
		return 0;
	}

	grapheSim *graphe_produit = produitGraphesCycles(g1, g2, sommets, taille);

	int* clique = cliqueMax(graphe_produit, (long)DATE);

	if (dot_option) {
    genererFichierDotGP(graphe_produit,sommets, clique, g1.chebi_id, g2.chebi_id);
  }

	int* taille_graphe_commun = grapheCommunG12(g1, g2, sommets, taille, clique);

	int nb_atomes_communs = taille_graphe_commun[0];	
	int nb_laisons_communs = taille_graphe_commun[1];


	float num = (float)((nb_atomes_communs + nb_laisons_communs) * (nb_atomes_communs + nb_laisons_communs));
	float denum = (float)((g1.nb_sommets + g1.nb_aretes) * (g2.nb_aretes + g2.nb_sommets));

	sim = num/denum;

	// Libérer la mémoire
	free(taille_graphe_commun);
	free(clique);
	free(sommets);
	for (int i = 0; i < graphe_produit->nb_sommets; i++) {
		free(graphe_produit->adjacence[i]);
	}
	free(graphe_produit->adjacence);
	free(graphe_produit);

	return sim;
}

float restrictionTailleCycles(int taille1, int taille2) {

	return (float)(ABS(taille1 - taille2)) <= 0.2 * MIN(taille1, taille2);
}

// Construit les couples de cycles compatibles pour 
// déterminer les sommets du graphe produit.
// La taille (= nombre de sommets) doit être initialisée à O 
// si on souhaite la déterminer dans la fonction.
couple* couplesCyclesCompatibles(grapheCycles g1, grapheCycles g2, int *taille) {
	
	int i, j;
	int n;
	*taille = 0;

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
	couple *couples = allouer(*taille * sizeof(couple), "tableau des couples de sommets du graphe produit (similarite.c)");

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

grapheSim* produitGraphesCycles(grapheCycles g1, grapheCycles g2, couple *sommets, int taille) { 

	int i, j;
	int i1, i2, j1, j2;

  int** adjacence =  allouer(taille * sizeof(int*), "matrice d'adjacence du graphe produit (similarite.c)");	
	for(i = 0; i < taille; i++) {
		adjacence[i] =  callouer(taille, sizeof(int), "matrice d'adjacence du graphe produit (similarite.c)");
	}

	for (i = 0; i < taille; i++) { 
		adjacence[i][i] = 1;

		i1 = sommets[i].id1;
		i2 = sommets[i].id2;

		for (j = i + 1; j < taille; j++) {
			j1=sommets[j].id1;
			j2=sommets[j].id2;
			
			if (g1.types_aretes[i1][j1].type == g2.types_aretes[i2][j2].type && 
					g1.types_aretes[i1][j1].poids == g2.types_aretes[i2][j2].poids) {
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
	
	grapheSim *g  = allouer(sizeof(grapheSim), "graphe produit (similarite.c)");
  g->nb_sommets = taille;
  g->adjacence = adjacence;

	return g;
}

// Contruit le graphe commun
int*  grapheCommunG12(grapheCycles g1, grapheCycles g2, couple *sommets, int taille, int* clique_max) {

	int i, j, i1, j1;
	int nb_sommets = 0, nb_aretes = 0;

	// Stocke le nombre de sommets et le nombre d'arêtes du graphe.
	int* taille_graphe_commun = (int*) allouer(2 * sizeof(int), "taille du graphe commun (similarite.c)");
	
	int tab[taille];

	for (i = 0; i < taille ; i++) {
		tab[i] = clique_max[i];
	}
	for (i = 0; i < taille - 1; i++) {
		if (tab[i] == 1) {
			for (j = i + 1; j < taille ; j++) {
				if (tab[j] == 1 && (sommets[i].id1 == sommets[j].id1)) {
					tab[j] = 0;
				}
			}
		}
	}
	for (i = 0; i < taille ; i++) {
		if (tab[i] == 1) {
			nb_sommets++;
		}
	}
	for (i = 0; i < taille - 1; i++) {
		if (tab[i] == 1) {
			for (j = i + 1; j < taille; j++) {
				if (tab[j] == 1) {
					i1 = sommets[i].id1;
					j1 = sommets[j].id1;
					if(g1.types_aretes[i1][j1].type != AUCUNE_LIAISON) {
						nb_aretes ++;
					}
				}
			}
		}
	}
	taille_graphe_commun[0] = nb_sommets;
	taille_graphe_commun[1] = nb_aretes;

	return taille_graphe_commun;
}
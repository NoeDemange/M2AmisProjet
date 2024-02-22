#include "similarite.h"
#include <stdio.h>
#include <stdlib.h>

int taille_clique_max = 0;
int *dans_clique_max= NULL;


int valeur_absolue(int a)
{
	if ( a > 0)
		return a;
	return -a;
}

int min( int a , int b)
{
	if ( a < b )
		return a;
	return b;
}

couple *construction_couples_cycles(grapheCycles a,grapheCycles b,int taille)
{//Construction des couples de cycles  compatibles
	
	int n = 0,i,j;
	
	couple *couple_at;
	couple_at = malloc(taille * sizeof(couple));

	for(i= 0; i < a.nb_sommets; i++) 
	{ 
		for(j= 0; j < b.nb_sommets;j++)
		{
			if( (float)(valeur_absolue(a.sommets[i].taille - b.sommets[j].taille)) <= 0.2 *min(a.sommets[i].taille ,b.sommets[j].taille))
			{
				couple_at[n].a1 = i;
				couple_at[n].a2 = j;
				n++;
			}
		}
	}
	//printf("%d %d", n , taille);
	return couple_at;
}

type_arete ** construction_matrice_graphe_cycles(grapheCycles a)
{//construction de la matrice de liaison d'une molecule

	int i,j;
	type_arete **matrice = NULL;

	matrice =  malloc(a.nb_sommets * sizeof(type_arete *));
	
	for(i=0; i < a.nb_sommets ;i++) matrice[i] =  malloc(a.nb_sommets * sizeof(type_arete));

	if(matrice  == NULL){
        fprintf(stdout,"Cannot allocate memory\n");
        exit(145);
    }


	for(i=0; i < a.nb_sommets ;i++)
	{
		for(j=0;j< a.nb_sommets;j++)
		{	
			matrice[i][j].type = AUCUNE_LIAISON;
			matrice[i][j].poids = AUCUNE_LIAISON;
			//printf("(%d,%d)", matrice[i][j].type,matrice[i][j].poids);
		}
		//printf("\n");
	}
	//printf("\n after \n");
	for(i =0; i< a.nb_aretes;i++)
	{	//printf("arêtes : %d, %d \n",a.aretes[i].id1, a.aretes[i].id2);
		matrice[a.aretes[i].id1][a.aretes[i].id2].type= a.aretes[i].type;
		matrice[a.aretes[i].id1][a.aretes[i].id2].poids= a.aretes[i].poids;
		matrice[a.aretes[i].id2][a.aretes[i].id1].type= a.aretes[i].type;
		matrice[a.aretes[i].id2][a.aretes[i].id1].poids= a.aretes[i].poids;
	}

	/*for(i=0; i < a.nb_sommets ;i++)
	{
		for(j=0;j< a.nb_sommets;j++)
		{	
			printf("(%d,%d)", matrice[i][j].type,matrice[i][j].poids);
		}
		printf("\n");
	}*/


	return matrice;
	
}

void liberer_type_arete(type_arete **m, grapheCycles a)
{
	int i;
	for(i= 0; i < a.nb_sommets; i++) free(m[i]);
	free(m);
}


graph graphe_produit_cycles(grapheCycles a,grapheCycles b){ 
    //prend en entrée deux graphes de cycles  et contruit le graphe produit
	int taille = 0;
	
	//calcul de la taille du graphe produit
	int i,j;
	for(i= 0; i < a.nb_sommets; i++) 
		for(j= 0; j < b.nb_sommets;j++)
			if( (float)(valeur_absolue(a.sommets[i].taille - b.sommets[j].taille)) <= 0.2 *min(a.sommets[i].taille ,b.sommets[j].taille)) ///A voir pourquoi float
				taille ++;
	
	//couple de liaisons entre les nouveaux sommets
	couple* couple_atome = construction_couples_cycles(a,b,taille);

	
	//construction de la matrice de liaison d'une molecule
	type_arete **m1 = construction_matrice_graphe_cycles(a);
	type_arete **m2 =construction_matrice_graphe_cycles(b);

   
  	int** matrice_liaisons = NULL;

	if( taille > 0)
	{	
		matrice_liaisons =  malloc(taille * sizeof(int *));	
		for(i = 0 ;i < taille;i++) 
    		matrice_liaisons[i] =  malloc(taille * sizeof(int));
	}
	
  //remplissage des liaisons
	int i1,i2,j1,j2;
	for(i= 0; i < taille ;i++)
		for(j= 0; j < taille ;j++)
			matrice_liaisons[i][j] = 0;

	for(i= 0; i < taille ;i++)
		matrice_liaisons[i][i] = 1;

	for(i= 0; i < taille ;i++){ 
		i1 = couple_atome[i].a1;
		i2 = couple_atome[i].a2;
		for(j= i + 1 ; j < taille ;j++){
			j1=couple_atome[j].a1;
			j2=couple_atome[j].a2;
			
			if( m1[i1][j1].type == m2[i2][j2].type && m1[i1][j1].poids == m2[i2][j2].poids){
				if(((i1 == j1) && (m2[i2][j2].type != AUCUNE_LIAISON)) || ((i2 == j2) && (m1[i1][j1].type != AUCUNE_LIAISON)) ||( (i1 != j1) && (i2!=j2))||( (i1 ==j1) && (i2==j2)) )
				{
					matrice_liaisons[i][j] = 1;
					matrice_liaisons[j][i] = 1;					
				}
			}
		}
	}
	if(couple_atome != NULL)
		free(couple_atome);
	liberer_type_arete(m1,a);
	liberer_type_arete(m2,b);
	
	graph f = build_graph_from_matrix(taille, matrice_liaisons);	

	return f;
}

int*  graphe_g12_cycles(graph g12, int* clique_max,grapheCycles a,grapheCycles b)
{ //contruction du graphe commun

	int* taille_graphe_commun = (int*) malloc(sizeof(int)*2);
  	int nb_at =0,nb_liaisons=0,i,j,i1,j1;
	
  	int taille = nbnodes(g12);
	couple * couple_atome = construction_couples_cycles(a,b,taille);
	type_arete **m1 = construction_matrice_graphe_cycles(a);
	
	int tab[taille];
	for(i=0;i < taille ; i++)
	{
		tab[i] = clique_max[i];
	}
	
	for(i=0;i < taille - 1; i++)
	{
		if(tab[i] == 1)
		{
			for(j=i+1;j < taille ; j++)
			{
				if(tab[j]==1 && (couple_atome[i].a1 == couple_atome[j].a1))
						tab[j] = 0;
			}
		}
	}

	for(i=0;i < taille ; i++)
	{
		if(tab[i] ==1)
			nb_at++;
	}

	for(i=0;i < taille - 1; i++)
	{
		if(tab[i] == 1)
		{
			for(j = i+1;j < taille; j++)
			{
				if(tab[j] == 1)
				{
					
					i1 = couple_atome[i].a1;
					j1 = couple_atome[j].a1;
					if(m1[i1][j1].type != AUCUNE_LIAISON)
						nb_liaisons ++;
				}
			}
		}
	}
	free(couple_atome);
	liberer_type_arete(m1,a);
	taille_graphe_commun[0] = nb_at;
	taille_graphe_commun[1] = nb_liaisons;
	return taille_graphe_commun;
	
}

float similarite(grapheCycles a,grapheCycles b)
{

	
	//int 
	float sim = 0.0;
	if(a.nb_sommets == 0 || b.nb_sommets == 0)
		sim = -1.0;
	else
	{
		//comence ici
		graph g = graphe_produit_cycles(a,b);

		int taille = nbnodes(g);
		//int** liaisons = build_matrix_from_graph(g);
		if(taille == 0) return 0;

		int* clique = clique_max(g, (long)date);

		int* taille_graphe_commun = graphe_g12_cycles(g,clique,a,b);

		int nb_atomes_communs = taille_graphe_commun[0];	
	  	int nb_laisons_communs = taille_graphe_commun[1];


	  	float num = (float)((nb_atomes_communs + nb_laisons_communs)*(nb_atomes_communs + nb_laisons_communs));
	  	float denum = (float)((a.nb_sommets + a.nb_aretes)*(b.nb_aretes + b.nb_sommets));
	  	sim = num/denum;
		free(taille_graphe_commun);
		free(clique);
		// //fin ici
		
		if(dans_clique_max != NULL)
		    free(dans_clique_max);
		taille_clique_max = 0;

		destroy(g);
	}	
	return sim;
}
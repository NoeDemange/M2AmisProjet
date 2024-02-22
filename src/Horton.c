#include "Horton.h"
#include "TriFusion.h"
#include "utiles.h"
#include "McKay.h"

//regarde si un sommet à au moins 2 voisins
bool check_cycle(int ** graph, int src,int nb_sommets){
    int trouve = 0;
    for (int i = 0; i < nb_sommets; i++){
        if(graph[src][i] !=0){
            trouve++;
            if(trouve == 2){
                return true;
            }
        }
    }
    return false;
}

bool Intersection(int* parents,int x, int y,int v){
    int chem1 = x;
    int chem2 = y;
    while (chem1 != v){
        chem2 = y;
        while (chem2 != v){
            if (chem1 == chem2 && chem1 != v){
                return false;
            }
            chem2 = parents[chem2];
        }
        chem1 = parents[chem1];
    }           
    return true;

}

cycle TransfoEnCycle(int v, int x, int y, int parents[], int V){


    cycle cycle;
    int* sommets = (int*)malloc(V * sizeof(int));
    int* index = (int*)malloc(V * sizeof(int));
    int chem1 = x;
    int chem2 = y;
    sommets[0] = v;
    int i = 0;
    while (chem2 != v){
        index[i] = chem2;
        chem2 = parents[chem2];
        i++;
    }
    int debut = 0;
    int fin = i - 1;

    while (debut < fin) {
        int temp = index[debut];
        index[debut] = index[fin];
        index[fin] = temp;

        debut++;
        fin--;
    }
    while (chem1 != v){
        index[i] = chem1;
        chem1 = parents[chem1];
        i++;
    }
    i--;
    for (int xi = 0; xi<=i; xi++){
        sommets[xi+1] = index[i-xi];
    }

    cycle.source = v;
    cycle.sommets = sommets;
    cycle.taille = i+2;
    free(index);

    return cycle;
}

cycle *ajouter_un_cycle(cycle *sets, int nb_cycles, cycle c){
	if( sets == NULL)
	{
		sets = malloc((nb_cycles + 1)* sizeof(cycle));

	}
	else
	{
		sets = realloc(sets, (nb_cycles + 1)* sizeof(cycle));
	}
	sets[nb_cycles] = c;
	
	return sets;
}

bool verification_ajout_cycle(cycle *sets, int nb_cycles , cycle c)
{
	int compteur ;	
	for(int i =  0; i < nb_cycles; i++)
	{
		
		if(sets[i].taille == c.taille)
		{
			compteur = 0;
			for(int j =  0; j < c.taille; j++)
			{
                for(int s =  0; s < sets[i].taille; s++)
			    {
				if(c.sommets[j] == sets[i].sommets[s])
				{
					compteur++;
				}
                }
			}
            //deux fois le même cycle
			if( compteur == c.taille)
			{
                free(c.sommets);
				return false;
			}
		}
	}
	return true;
}

//Donne tous les arcs du graphes dans un tableau
Edge* obtenirArcs(int** graph, int* numArcs,int nb_sommets) {
    Edge* edges = (Edge*)malloc(nb_sommets * nb_sommets * sizeof(Edge)); 
    *numArcs = 0;

    for (int i = 0; i < nb_sommets; i++) {
        for (int j = i + 1; j < nb_sommets; j++) {
            if (graph[i][j] != 0) {
                edges[*numArcs].source = i;
                edges[*numArcs].target = j;
                edges[*numArcs].poids = graph[i][j];
                (*numArcs)++;
            }
        }
    }
    for(int aff=0; aff<*numArcs;aff++){
        printf("%d,%d ",edges[aff].source, edges[aff].target);
    }
    printf("\n");

    return edges;
}

int** Marquage(cycle* set,int nb_cycles,int** graph,int nb_arcs, Edge * edges){
    int** edgeMatrix = (int**)malloc(nb_cycles * sizeof(int*));

    for (int z = 0; z < nb_cycles; z++) {
        edgeMatrix[z] = (int*)calloc(nb_arcs, sizeof(int));
    }


    for (int i = 0; i < nb_cycles; i++) {
        for (int j = 0; j < set[i].taille; j++){
            for (int x = 0; x < nb_arcs; x ++){
                if((set[i].sommets[j] == edges[x].source && set[i].sommets[(j+1)%set[i].taille] == edges[x].target) ||
                (set[i].sommets[j] == edges[x].target && set[i].sommets[(j+1)%set[i].taille] == edges[x].source)
                ){
                    edgeMatrix[i][x] = 1;
                    break;
                }
            }

        }
    }
    return edgeMatrix;
}


int *produit_xor_matrice(int *t, int *tab1, int *tab2,int nb){
	t= malloc(nb*sizeof(int));
	int i;
	for ( i = 0; i < nb; i++)
		t[i] = tab1[i]^tab2[i];
	return t;
	
}

//verifie si deux tableaux sont les memes
int verification_egalite_tableaux(int *tab1,int *tab2 , int taille){
	int i, verif = 1 ;
	for( i = 0; i < taille ; i++)
	{
		if( tab1[i] != tab2[i])
		{
			verif = 0;
			break;
		}
	}
	return verif;
}

cycle* Elimination_Gaussienne(cycle* set,int *nb_cycles,int** graph,int nb_sommets){
    int nb_arcs;
    Edge* edges = obtenirArcs(graph,&nb_arcs,nb_sommets);
    int ** edgeMatrix = Marquage(set,*nb_cycles,graph,nb_arcs,edges);

    // ///////AFFICHAGE////////
    for(int xi = 0; xi< (*nb_cycles) ; xi++){
        for(int xj = 0; xj< nb_arcs; xj++){
            printf("%d ",edgeMatrix[xi][xj]);
        }
        printf("\n");
    }
    printf("\n");

    int ** save = malloc((*nb_cycles)*sizeof(int*));
    for(int s = 0; s<(*nb_cycles);s++){
        save[s] = malloc(nb_arcs*sizeof(int));
        for(int sj=0; sj<nb_arcs;sj++){
            save[s][sj] = edgeMatrix[s][sj];
        }
    }

    // Gaussian Elimination
    int first;
        for (int i = 0; i < (*nb_cycles) - 1; i++)
        {
            //trouver le premier un 
            first = -1;
            for (int j= 0; j < nb_arcs; j++)
            {
                if(edgeMatrix[i][j] == 1)
                {
                    first = j;
                    break;
                }
            }
            if(first != -1)
            {
                for (int k = i+1; k < (*nb_cycles); k++)
                {
                    if( edgeMatrix[k][first] == 1)
                    {
                        for (int l = 0; l < nb_arcs;l++)
                        {
                            //Xor
                            edgeMatrix[k][l] ^=  edgeMatrix[i][l];
                        }
                    }
                }
            }
        }

    ///////AFFICHAGE////////
    for(int xi = 0; xi< (*nb_cycles) ; xi++){
        for(int xj = 0; xj< nb_arcs; xj++){
            printf("%d ",edgeMatrix[xi][xj]);
        }
        printf("\n");
    }

    // Count the number of independent cycles
   // int independent_cycles = 0;
    int tab_base[(*nb_cycles)];
    for (int i = 0; i < (*nb_cycles); i++) {
        tab_base[i] = 0;
        for (int j = 0; j < nb_arcs; j++) {
            if (edgeMatrix[i][j] == 1) {
                tab_base[i] = 1;
                break;
            }
        }
        //if (independent) {
        //    independent_cycles++;
        //}
    }

int *aj_cycle,maxi;
    for(int i = 0; i<(*nb_cycles)-1;i++){
        for(int j =i+1; j<(*nb_cycles);j++){
            if(tab_base[i]*tab_base[j]==1){
                aj_cycle=produit_xor_matrice(aj_cycle,save[i],save[j],nb_arcs);
                for(int l =0; l<(*nb_cycles);l++){
                    if(set[i].taille > set[j].taille){
						maxi = set[i].taille;
                    }
					else{
						maxi = set[j].taille;
                    }
                    if(verification_egalite_tableaux(aj_cycle,save[l],nb_arcs) == 1 && tab_base[l] == 0 && set[l].taille == maxi){
                            tab_base[l] = 1;
                            break;
					}
                }
                free(aj_cycle);
            }
        }
    }

    cycle* base_cycle = NULL;
    int cycle_ind =0;
    for(int i = 0; i < (*nb_cycles);i++)
	{
		if(tab_base[i] == 1)
		{
			base_cycle = ajouter_un_cycle(base_cycle, cycle_ind, set[i]);
            cycle_ind++;
		}
	}



    // // Create the minimum basis
    // int ** minimum_basis = (int**)malloc(independent_cycles * sizeof(int*));
    // int basis_index = 0;
    // for (int i = 0; i < (*nb_cycles); i++) {
    //     bool independent = false;
    //     for (int j = 0; j < nb_arcs; j++) {
    //         if (edgeMatrix[i][j] == 1) {
    //             independent = true;
    //             break;
    //         }
    //     }
    //     if (independent) {
    //         minimum_basis[basis_index] = (int*)malloc(nb_arcs * sizeof(int));
    //         for (int j = 0; j < nb_arcs; j++) {
    //             minimum_basis[basis_index][j] = edgeMatrix[i][j];
    //         }
    //         basis_index++;
    //     }
    // }

    //  ///////AFFICHAGE////////
    //  printf("Indep %d\n", independent_cycles);
    // for(int xi = 0; xi< independent_cycles ; xi++){
    //     for(int xj = 0; xj< nb_arcs; xj++){
    //         printf("%d ",minimum_basis[xi][xj]);
    //     }
    //     printf("\n");
    // }

    // Free memory
    for (int i = 0; i < (*nb_cycles); i++) {
        free(edgeMatrix[i]);
        free(save[i]);
    }
    free(save);
    free(edgeMatrix);
    free(edges);
    *(nb_cycles)=cycle_ind;
    return base_cycle;
}

int ** Horton(int** graph,int nb_sommets){
    int* chemins = (int*)malloc(nb_sommets * sizeof(int));
    int* parents = (int*)malloc(nb_sommets * sizeof(int)); 
    int v = 0;
    int nb_cycles = 0;
    cycle* sets = NULL;
    cycle c;
    for (v = 0; v < nb_sommets; v ++){
        if(check_cycle(graph,v,nb_sommets)){
            dijkstra(graph, v, chemins, parents,nb_sommets);
            for (int x = 0; x < nb_sommets; x++) {
                if (x != v && check_cycle(graph,x,nb_sommets)){

                    for (int y = x + 1; y < nb_sommets; y++) {
                        if (y!= v && check_cycle(graph,x,nb_sommets)){
                            if (Intersection(parents,x,y,v) && graph[x][y] != 0) {
                                c = TransfoEnCycle(v, x, y, parents,nb_sommets);
                                if (verification_ajout_cycle(sets,nb_cycles,c)){
                                    sets = ajouter_un_cycle(sets, nb_cycles, c);
                                    nb_cycles++;    
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    free(chemins);
    free(parents);
    triFusion(sets, nb_cycles);

    ///////AFFICHAGE////////
    for(int affS = 0; affS< nb_cycles; affS++){
        printf("Cycle src: %d, taille: %d, sommets:",sets[affS].source, sets[affS].taille);
        for(int aff = 0; aff<sets[affS].taille; aff++){
            printf("%d,",sets[affS].sommets[aff]);
        }
        printf("\n");
    }

    cycle* base_de_cycle = Elimination_Gaussienne(sets, &nb_cycles, graph,nb_sommets);
    
    int nb_arcs;
    Edge* edges = obtenirArcs(graph,&nb_arcs,nb_sommets);
    free(edges);
    ///////AFFICHAGE////////
    printf("fin \n");
    for(int affS = 0; affS< nb_cycles; affS++){
        printf("Cycle src: %d, taille: %d, sommets:",base_de_cycle[affS].source, base_de_cycle[affS].taille);
        for(int aff = 0; aff<base_de_cycle[affS].taille; aff++){
            printf("%d,",base_de_cycle[affS].sommets[aff]);
        }
        printf("\n");
    }
    /*for(int xi = 0; xi< nb_cycles ; xi++){
        for(int xj = 0; xj< nb_arcs; xj++){
            printf("%d ",base_de_cycle[xi][xj]);
        }
        printf("\n");
    }*/
    /*for (int xi = 0; xi<nb_cycles ; xi++){
        free(sets[xi].sommets);
        free(base_de_cycle[xi]);
    }*/
    //free(base_de_cycle);
    free(sets);
    int** base=NULL;
    return base;
}

void Test_Horton(){
    int **graph;
    grapheMol graphe_mol;
    graphe_mol.chebi_id = 1729;
    graphe_mol.nb_sommets = 22;
    graph = malloc(graphe_mol.nb_sommets * sizeof(int*));

    int graph_init[22][22] = {
        {0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0},
{0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1},
{0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0}
    };

    /*graphe_mol.chebi_id = 15854;
    graphe_mol.nb_sommets = 19;
    graph = malloc(graphe_mol.nb_sommets * sizeof(int*));

    int graph_init[19][19] = {
        {0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,1,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0},
        {1,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,1},
        {0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0},
        {0,0,0,0,0,0,0,0,0,0,1,0,1,1,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,1,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,1},
        {0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0}};*/


    for (int i = 0; i < graphe_mol.nb_sommets; i++) {
        graph[i] = malloc(graphe_mol.nb_sommets * sizeof(int));
        for (int j = 0; j < graphe_mol.nb_sommets; j++) {
            graph[i][j] = graph_init[i][j];
        }
    }

    graphe_mol.adjacence = graph;

    grapheCanonique(&graphe_mol);
    //printGrapheMol(graphe_mol);

    Horton(graphe_mol.adjacence,graphe_mol.nb_sommets);
    for (int i = 0; i < graphe_mol.nb_sommets; i++) {
        free(graph[i]);
    }
    free(graph);
}
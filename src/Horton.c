#include "Horton.h"
#include "TriFusion.h"

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


Cycle TransfoEnCycle(int v, int x, int y, int parents[], int V){


    Cycle cycle;
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
Cycle *ajouter_un_cycle(Cycle *sets, int nb_cycles, Cycle c){
	if( sets == NULL)
	{
		sets = malloc((nb_cycles + 1)* sizeof(Cycle));

	}
	else
	{
		sets = realloc(sets, (nb_cycles + 1)* sizeof(Cycle));
	}
	sets[nb_cycles] = c;
	
	return sets;
}

bool verification_ajout_cycle(Cycle *sets, int nb_cycles , Cycle c)
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

int** Marquage(Cycle* set,int nb_cycles,int** graph,int nb_arcs, Edge * edges){
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


int ** Elimination_Gaussienne(Cycle* set,int *nb_cycles,int** graph,int nb_sommets){
    int nb_arcs;
    Edge* edges = obtenirArcs(graph,&nb_arcs,nb_sommets);
    int ** edgeMatrix = Marquage(set,*nb_cycles,graph,nb_arcs,edges);

    // ///////AFFICHAGE////////
    // for(int xi = 0; xi< (*nb_cycles) ; xi++){
    //     for(int xj = 0; xj< nb_arcs; xj++){
    //         printf("%d ",edgeMatrix[xi][xj]);
    //     }
    //     printf("\n");
    // }
    // printf("\n");

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
    int independent_cycles = 0;
    for (int i = 0; i < (*nb_cycles); i++) {
        bool independent = false;
        for (int j = 0; j < nb_arcs; j++) {
            if (edgeMatrix[i][j] == 1) {
                independent = true;
                break;
            }
        }
        if (independent) {
            independent_cycles++;
        }
    }

    // Create the minimum basis
    int ** minimum_basis = (int**)malloc(independent_cycles * sizeof(int*));
    int basis_index = 0;
    for (int i = 0; i < (*nb_cycles); i++) {
        bool independent = false;
        for (int j = 0; j < nb_arcs; j++) {
            if (edgeMatrix[i][j] == 1) {
                independent = true;
                break;
            }
        }
        if (independent) {
            minimum_basis[basis_index] = (int*)malloc(nb_arcs * sizeof(int));
            for (int j = 0; j < nb_arcs; j++) {
                minimum_basis[basis_index][j] = edgeMatrix[i][j];
            }
            basis_index++;
        }
    }

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
    }
    free(edgeMatrix);
    free(edges);
    *(nb_cycles)=independent_cycles;
    return minimum_basis;
}

int ** Horton(int** graph,int nb_sommets){
    int* chemins = (int*)malloc(nb_sommets * sizeof(int));
    int* parents = (int*)malloc(nb_sommets * sizeof(int)); 
    int v = 0;
    int nb_cycles = 0;
    Cycle* sets = NULL;
    Cycle c;
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
    ///////AFFICHAGE////////
    for(int affS = 0; affS< nb_cycles; affS++){
        printf("Cycle src: %d, taille: %d, sommets:",sets[affS].source, sets[affS].taille);
        for(int aff = 0; aff<sets[affS].taille; aff++){
            printf("%d,",sets[affS].sommets[aff]);
        }
        printf("\n");
    }


    free(chemins);
    free(parents);
    triFusion(sets, nb_cycles);
    int ** base_de_cycle = Elimination_Gaussienne(sets, &nb_cycles, graph,nb_sommets);
    
    int nb_arcs;
    Edge* edges = obtenirArcs(graph,&nb_arcs,nb_sommets);
    free(edges);
    ///////AFFICHAGE////////
    printf("fin \n");
    for(int xi = 0; xi< nb_cycles ; xi++){
        for(int xj = 0; xj< nb_arcs; xj++){
            printf("%d ",base_de_cycle[xi][xj]);
        }
        printf("\n");
    }
    for (int xi = 0; xi<nb_cycles ; xi++){
        free(sets[xi].sommets);
        free(base_de_cycle[xi]);
    }
    //free(base_de_cycle);
    free(sets);
    return base_de_cycle;
}

void Test_Horton(){
    int **graph;
    int V = 6;
    graph = malloc(V * sizeof(int*));
    int graph_init[6][6] = {
        {0, 4, 0, 0, 1, 0},
        {4, 0, 8, 0, 0, 0},
        {0, 8, 0, 7, 0, 4},
        {0, 0, 7, 0, 9, 14},
        {1, 0, 0, 9, 0, 10},
        {0, 0, 4, 14, 10, 0}
    };
    /*int graph_init[6][6] = {
        {0, 1, 0, 0, 0, 1},
        {1, 0, 1, 0, 1, 0},
        {0, 1, 0, 1, 0, 0},
        {0, 0, 1, 0, 1, 0},
        {0, 1, 0, 1, 0, 1},
        {1, 0, 0, 0, 1, 0}
    };*/

    for (int i = 0; i < V; i++) {
        graph[i] = malloc(V * sizeof(int));
        for (int j = 0; j < V; j++) {
            graph[i][j] = graph_init[i][j];
        }
    }
    Horton(graph,V);
    for (int i = 0; i < V; i++) {
        free(graph[i]);
    }
    free(graph);
}
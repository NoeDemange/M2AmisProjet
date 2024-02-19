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
    free(sommets);
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
	int i,j;
	
	int compteur ;	
	for( i =  0; i < nb_cycles; i++)
	{
		
		if(sets[i].taille == c.taille)
		{
			compteur = 0;
			for( j =  0; j < c.taille; j++)
			{
				if(c.sommets[j] == sets[i].sommets[j])
				{
					compteur++;
				}
			}
            //deux fois le même cycle
			if( compteur == c.taille)
			{
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

    return edges;
}

int** Marquage(Cycle* set,int nb_cycles,int** graph,int nb_arcs, Edge * edges){
    int** edgeMatrix = (int**)malloc(nb_cycles * sizeof(int*));

    for (int z = 0; z < nb_cycles; z++) {
        edgeMatrix[z] = (int*)calloc(nb_arcs, sizeof(int));
    }


    for (int i = 0; i < nb_cycles; i++) {
        for (int j = 0; j+1 < set[i].taille; j++){
            for (int x = 0; x < nb_arcs; x ++){
                if(set[i].sommets[j] == edges[x].source && set[i].sommets[j+1] == edges[x].target){
                    edgeMatrix[i][x] = 1;
                    break;
                }
            }

        }
    }
    return edgeMatrix;
}


int ** Elimination_Gaussienne(Cycle* set,int nb_cycles,int** graph,int nb_sommets){
    int nb_arcs;
    Edge* edges = obtenirArcs(graph,&nb_arcs,nb_sommets);
    int ** edgeMatrix = Marquage(set,nb_cycles,graph,nb_arcs,edges);
    int i,j,k,l;
    int first;
        for (i = 0; i < nb_cycles - 1; i++)
        {
            //trouver le premier un 
            first = -1;
            for (j= 0; j < nb_arcs; j++)
            {
                if(edgeMatrix[i][j] == 1)
                {
                    first = j;
                    break;
                }
            }
            if(first != -1)
            {
                for ( k = i+1; k < nb_cycles; k++)
                {
                    if( edgeMatrix[k][first] == 1)
                    {
                        for ( l = 0; l < nb_arcs;l++)
                        {
                            //Xor
                            edgeMatrix[k][l] ^= edgeMatrix[k][l] & edgeMatrix[i][l];
                        }
                    }
                }
            }
        }
        free(edges);
        return edgeMatrix;
}

int ** Horton(int** graph,int nb_sommets){
    int* chemins = (int*)malloc(nb_sommets * sizeof(int));
    int* parents = (int*)malloc(nb_sommets * sizeof(int)); 
    int v = 0;
    int i = 0;
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
                                if (verification_ajout_cycle(sets,i,c)){
                                    sets = ajouter_un_cycle(sets, i, c);
                                    i++;    
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
    triFusion(sets, i);
    int ** base_de_cycle = Elimination_Gaussienne(sets, i, graph,nb_sommets);
    
    int nb_arcs;
    Edge* edges = obtenirArcs(graph,&nb_arcs,nb_sommets);
    free(edges);

    for(int xi = 0; xi< i ; xi++){
        for(int xj = 0; xj< nb_arcs; xj++){
            printf("%d ",base_de_cycle[xi][xj]);
        }
        printf("\n");
    }
    for (int xx = 0; xx<i ; xx++){
        free(base_de_cycle[xx]);
    }
    free(base_de_cycle);
    free(sets);
    return base_de_cycle;
}

void Test_Horton(){
    int **graph;
    int V = 6;
    graph = malloc(V * sizeof(int*));
    int graph_init[6][6] = {
        {0, 4, 0, 0, 0, 0},
        {4, 0, 8, 0, 0, 0},
        {0, 8, 0, 7, 0, 4},
        {0, 0, 7, 0, 9, 14},
        {0, 0, 0, 9, 0, 10},
        {0, 0, 4, 14, 10, 0}
    };
    
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
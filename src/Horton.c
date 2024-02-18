#include "Horton.h"
#include "TriFusion.h"

bool Intersection(int* chemins, int* parents,int x, int y){
    int chem1 = x;
    int chem2 = y;

    while (chemins[chem1] != 0){
        chem2 = y;
        while (chemins[chem2] != 0){

            if (parents[chem1] == parents[chem2]){
                return false;
            }
            chem2 = parents[chem2];
        }
        chem1 = parents[chem1];
    }           

    return true;

}

Cycle TransfoEnCycle(int v, int x, int y, int parents[], int graph[V][V]){
    Cycle cycle;
    int* sommets = (int*)malloc(V * sizeof(int));
    int* index = (int*)malloc(V * sizeof(int));
    int chem1 = x;
    int chem2 = y;
    sommets[0] = v;
    int i,j = 0;
    while (parents[chem1] != v){
        index[i] = chem1;
        i++;
        chem1 = parents[chem1];
    }   
    index[i] = chem1;
    for (int xi = 0; xi<=i; xi++){
        sommets[xi+1] = index[i-xi];
    }

    while (parents[chem2] != v){
        index[j] = chem2;
        j++;
        chem2 = parents[chem2];
    }
    index[j] = chem2;
    for (int xi = 0; xi<=i; xi++){
        sommets[xi+1+i] = index[j-xi];
    }        

    cycle.source = v;
    cycle.sommets = sommets;
    cycle.taille = i + j +1;
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
Edge* obtenirArcs(int graph[V][V], int* numArcs) {
    Edge* edges = (Edge*)malloc(V * V * sizeof(Edge)); 
    *numArcs = 0;

    for (int i = 0; i < V; i++) {
        for (int j = i + 1; j < V; j++) {
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

int** Marquage(Cycle* set,int nb_cycle,int graph[V][V]){
    int nb_arcs;
    Edge* edges = obtenirArcs(graph,&nb_arcs);
    int** edgeMatrix = (int**)malloc(nb_cycle * sizeof(int*));

    for (int z = 0; z < nb_cycle; z++) {
        edgeMatrix[z] = (int*)calloc(nb_arcs, sizeof(int));
    }


    for (int i = 0; i < nb_cycle; i++) {
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


Cycle Horton(int graph[V][V]){
    int* chemins = (int*)malloc(V * sizeof(int));
    int* parents = (int*)malloc(V * sizeof(int)); 
    int v = 0;
    int i = 0;
    Cycle* sets = NULL;
    Cycle c;
    for (v = 0; v < V; v ++){
        dijkstra(graph, v, chemins, parents);
        for (int x = 0; x < V; x++) {
            for (int y = x + 1; y < V; y++) {
                if (Intersection(chemins,parents,x,y) && x != v && y !=v) {
                    c = TransfoEnCycle(v, x, y, parents, graph);
                    if (verification_ajout_cycle(sets,i,c)){
                        sets = ajouter_un_cycle(sets, i, c);
                        i++;    
                    }
                }
            }
        }
    }
    free(chemins);
    free(parents);
    free(sets);
    triFusion(sets, i);
    return sets[0];
}
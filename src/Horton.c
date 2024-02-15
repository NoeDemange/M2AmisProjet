#include "Horton.h"
#include "TriFusion.h"

Cycle TransfoEnCycle(int v, int x, int y, int chemins[], int parents[], int graph[V][V]){
    Cycle cycle;
    int* sommets = (int*)malloc(V * sizeof(int));
    int i = 1;
    sommets[0] = y;
    int z = y;
    while (sommets[i] != x){
        sommets[i] = parents[z];
        z = parents[z];
        i++;
    }
    while (sommets[i] != v){
        sommets[i] = parents[z];
        z = parents[z];
        i++;
    }
    cycle.source = v;
    cycle.parents = sommets;
    cycle.taille = i;
    free(sommets);

    return cycle;
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
                if(set[i].parents[j] == edges[x].source && set[i].parents[j+1] == edges[x].target){
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
    Cycle sets[V];
    for (v = 0; v < V; v ++){
        dijkstra(graph, v, chemins, parents);
        for (int x = 0; x < V; x++) {
            for (int y = x + 1; y < V; y++) {
                if (chemins[v] + graph[v][x] == chemins[x] && chemins[v] + graph[v][y] == chemins[y]) {
                    sets[i] = TransfoEnCycle(v, x, y, chemins, parents,graph);
                    i++;
                }
            }
        }
    }
    free(chemins);
    free(parents);
    triFusion(sets, i);
    return sets[0];
}
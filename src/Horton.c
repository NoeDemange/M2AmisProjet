#include "Horton.h"


typedef struct {
    int source;
    int* parents;
    int poids; 
} SetCycle;

SetCycle TransfoEnCycle(int v, int x, int y, int chemins[], int parents[], int graph[V][V]){
    SetCycle cycle;
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
    cycle.poids = chemins[x] + chemins[y] + graph[x][y];
    free(sommets);

    return cycle;
}

SetCycle Horton(int graph[V][V]){
    int* chemins = (int*)malloc(V * sizeof(int));
    int* parents = (int*)malloc(V * sizeof(int)); 
    int v = 0;
    int i = 0;
    SetCycle sets[V];
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
    return sets[0];// on est censé renvoyé le plus petit cycle mais je n'ai pas encore implémenté le tri
}
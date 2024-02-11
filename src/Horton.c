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
    return sets[0];// on est censé renvoyé le plus petit cycle mais je n'ai pas encore implémenté le tri
}
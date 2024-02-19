#include <stdio.h>
#include <stdbool.h>
#include <limits.h>
#include "Dijkstra.h"


// Trouve et renvoie le sommet le plus proche non visité
int minDistance(int dist[], bool visited[],int nb_sommets) {
    int min = INT_MAX, min_index = -1;
    for (int v = 0; v < nb_sommets; v++) {
        if (visited[v] == false && dist[v] <= min && dist[v] != INT_MAX) {
            min = dist[v];
            min_index = v;
        }
    }
    return min_index;
}


void dijkstra(int ** graph, int src, int dijkstra[], int parents[],int nb_sommets) {
    bool* visited = malloc(nb_sommets * sizeof(bool));
    for (int i = 0; i < nb_sommets; i++) {
        dijkstra[i] = INT_MAX;
        visited[i] = false;
        parents[i] = -1;
    }

    dijkstra[src] = 0; 

    for (int count = 0; count < nb_sommets - 1; count++) {

        int u = minDistance(dijkstra, visited,nb_sommets);
        if (u != -1){
            visited[u] = true;

            for (int v = 0; v < nb_sommets; v++) {
                if (!visited[v] && graph[u][v] && dijkstra[u] != INT_MAX &&
                dijkstra[u] + graph[u][v] < dijkstra[v]) {
                    dijkstra[v] = dijkstra[u] + graph[u][v];
                    parents[v] = u;
                }
            }
        }
    }
    free(visited);
}

void test_dijkstra(){
    int **graph;
    int V = 6;
    int* chemins = (int*)malloc(V * sizeof(int));
    int* parents = (int*)malloc(V * sizeof(int)); 
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
    dijkstra(graph,2,chemins,parents,V);
    for (int i = 0; i < V; i++) {
        printf("Depuis sommet %d le sommet plus proche pour aller à src=  %d\n",i, parents[i]);
        free(graph[i]);
    }
    
    free(chemins);
    free(parents);
    free(graph);
}
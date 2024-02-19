#include <stdio.h>
#include <stdbool.h>
#include <limits.h>
#include "Dijkstra.h"


// Trouve et renvoie le sommet le plus proche non visité
int minDistance(int dist[], bool visited[],int nb_sommets) {
    int min = INT_MAX, min_index = -1;
    for (int v = 0; v < nb_sommets; v++) {
        if (visited[v] == false && dist[v] <= min && dist[v] != -1) {
            min = dist[v];
            min_index = v;
        }
    }
    return min_index;
}


void dijkstra(int ** graph, int src, int dijkstra[], int parents[],int nb_sommets) {
    bool* visited = malloc(nb_sommets * sizeof(bool));
    for (int i = 0; i < nb_sommets; i++) {
        dijkstra[i] = -1;
        visited[i] = false;
        parents[i] = -1;
    }

    dijkstra[src] = 0; 

    for (int count = 0; count < nb_sommets - 1; count++) {
        int u = minDistance(dijkstra, visited,nb_sommets);
        visited[u] = true;

        for (int v = 0; v < nb_sommets; v++) {
            if (!visited[v] && graph[u][v] && dijkstra[u] != -1 &&
            dijkstra[u] + graph[u][v] < dijkstra[v]) {
                dijkstra[v] = dijkstra[u] + graph[u][v];
                parents[v] = u;
            }
        }
    }
    free(visited);
}

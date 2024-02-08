#include <stdio.h>
#include <stdbool.h>
#include <limits.h>
#include "Dijkstra.h"

#define V 6 // Nombre de sommets(à changer plus tard pour que ce soit modulable facilement)

// Trouve et renvoie le sommet le plus proche non visité
int minDistance(int dist[], bool visited[]) {
    int min = INT_MAX, min_index;
    for (int v = 0; v < V; v++) {
        if (visited[v] == false && dist[v] <= min && dist[v] != -1) {
            min = dist[v];
            min_index = v;
        }
    }
    return min_index;
}


void dijkstra(int graph[V][V], int src, int dijkstra[], int parents[]) {
    bool visited[V];
    for (int i = 0; i < V; i++) {
        dijkstra[i] = -1;
        visited[i] = false;
        parents[i] = -1;
    }

    dijkstra[src] = 0; 

    for (int count = 0; count < V - 1; count++) {
        int u = minDistance(dijkstra, visited);
        visited[u] = true;

        for (int v = 0; v < V; v++) {
            if (!visited[v] && graph[u][v] && dijkstra[u] != -1 &&
            dijkstra[u] + graph[u][v] < dijkstra[v]) {
                dijkstra[v] = dijkstra[u] + graph[u][v];
                parents[v] = u;
            }
        }
    }
}

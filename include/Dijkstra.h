#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include <stdio.h>
#include <stdbool.h>
#include <limits.h>

#define V 6

int minDistance(int dist[], bool visited[]);
void dijkstra(int graph[V][V], int src, int shortestDistances[], int parents[]);

#endif

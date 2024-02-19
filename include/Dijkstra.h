#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include <stdio.h>
#include <stdbool.h>
#include <limits.h>
#include <stdlib.h>


int minDistance(int dist[], bool visited[],int nb_sommets);
void dijkstra(int** graph, int src, int shortestDistances[], int parents[],int nb_sommets);

#endif

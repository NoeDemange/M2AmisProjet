#ifndef HORTON_H
#define HORTON_H

#include "Dijkstra.h"
#include "TriFusion.h"
#define V 6   // Nb de sommets du graph

typedef struct {
    int source;
    int target;
    int poids; 
} Edge;

Cycle TransfoEnCycle(int v, int x, int y, int chemins[], int parents[],int graph[V][V]);
Cycle Horton(int graph[V][V]);

#endif
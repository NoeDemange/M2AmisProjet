#ifndef HORTON_H
#define HORTON_H

#include "Dijkstra.h"
#include "TriFusion.h"
#define V 6   // Nb de sommets du graph

Cycle TransfoEnCycle(int v, int x, int y, int chemins[], int parents[],int graph[][]);
Cycle Horton(int graph[][]);

#endif
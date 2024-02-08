#ifndef HORTON_H
#define HORTON_H

#include "Dijkstra.h"
#define V 6  /* Define the value of V here */   // Assuming V is a defined constant

typedef struct {
    int source;
    int* parents;
    int poids; 
} SetCycle;

SetCycle TransfoEnCycle(int v, int x, int y, int chemins[], int parents[],int graph[][]);
SetCycle Horton(int graph[][]);

#endif
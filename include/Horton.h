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

bool Intersection(int* chemins, int* parents,int x, int y);
Cycle TransfoEnCycle(int v, int x, int y, int parents[],int graph[V][V]);
Cycle *ajouter_un_cycle(Cycle *sets, int nb_cycles, Cycle c);
bool verification_ajout_cycle(Cycle *sets, int nb_cycles , Cycle c);
Edge* obtenirArcs(int graph[V][V], int* numArcs);
int** Marquage(Cycle* set,int nb_cycle,int graph[V][V]);
Cycle Horton(int graph[V][V]);

#endif
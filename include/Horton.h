#ifndef HORTON_H
#define HORTON_H

#include "Dijkstra.h"
#include "TriFusion.h"

typedef struct {
    int source;
    int target;
    int poids; 
} Edge;

bool Intersection(int* parents,int x, int y,int v);
cycle TransfoEnCycle(int v, int x, int y, int parents[],int V);
cycle *ajouter_un_cycle(cycle *sets, int nb_cycles, cycle c);
bool verification_ajout_cycle(cycle *sets, int nb_cycles , cycle c);
Edge* obtenirArcs(int** graph, int* numArcs,int nb_sommets);
int** Marquage(cycle* set,int nb_cycle,int** graph,int nb_arcs,Edge * edges);
cycle* Elimination_Gaussienne(cycle* set,int *nb_cycles,int** graph,int nb_sommets);
int** Horton(int** graph,int nb_sommets);
void Test_Horton();

#endif
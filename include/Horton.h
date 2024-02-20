#ifndef HORTON_H
#define HORTON_H

#include "Dijkstra.h"
#include "TriFusion.h"

typedef struct {
    int source;
    int target;
    int poids; 
} Edge;

bool check_cycle(int ** graph,int v,int nb_sommets);
bool Intersection(int* parents,int x, int y,int v);
Cycle TransfoEnCycle(int v, int x, int y, int parents[],int V);
Cycle *ajouter_un_cycle(Cycle *sets, int nb_cycles, Cycle c);
bool verification_ajout_cycle(Cycle *sets, int nb_cycles , Cycle c);
Edge* obtenirArcs(int** graph, int* numArcs,int nb_sommets);
int** Marquage(Cycle* set,int nb_cycle,int** graph,int nb_arcs,Edge * edges);
int ** Elimination_Gaussienne(Cycle* set,int *nb_cycles,int** graph,int nb_sommets);
int** Horton(int** graph,int nb_sommets);
void Test_Horton();

#endif
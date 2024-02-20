#ifndef GRAPHECYCLE_H
#define GRAPHECYCLE_H

#include "structure.h"

void marquerAretesCycles(grapheMol *g, listeCycles *liste_c);
void trierSommetsCycles(listeCycles *liste_c, indexCycles *index_cycles);
int intersectionCycles(cycle c1, cycle c2);
void ajouterAreteCyclesJoints(listeCycles *liste_c, listeAretes **aretes, int *nb_aretes);

#endif
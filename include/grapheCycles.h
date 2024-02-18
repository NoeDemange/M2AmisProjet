#ifndef GRAPHECYCLE_H
#define GRAPHECYCLE_H

#include "structure.h"

void marquerAretesCycles(grapheMol *g, listeCycles *liste_c);
listeAretes* trouverIsthmes(grapheMol g, int *nb_isthmes);
void trierSommetsCycles(listeCycles *liste_c, indexCycles *index_cycles);
int intersectionCycles(Cycle c1, Cycle c2);
void ajouterAreteCyclesJoints(listeCycles *liste_c, listeAretes **aretes, int *nb_aretes);

#endif
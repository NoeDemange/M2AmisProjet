#ifndef GRAPHECYCLE_H
#define GRAPHECYCLE_H

#include <stdio.h>
#include <stdlib.h>

#include "TriFusion.h"
#include "utiles.h"
#include "structure.h"

grapheCycles transfoGrapheCycles(grapheMol graphe_mol, listeCycles *cycles, indexCycles *index_cycles);
void marquerAretesCycles(grapheMol *g, listeCycles *cycles, indexCycles *index_cycles);
void trierSommetsCycles(listeCycles *liste_c);
int intersectionCycles(cycle c1, cycle c2);
void ajouterAreteCyclesJoints(listeCycles *liste_c, listeAretes **aretes, int *nb_aretes);
void ajouterAreteCyclesDisjoints(listeCycles *liste_c, listeAretes **aretes, int *nb_aretes, indexCycles *index_cycles, grapheMol g);

#endif
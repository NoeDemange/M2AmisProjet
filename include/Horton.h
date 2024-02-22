#ifndef HORTON_H
#define HORTON_H

#include "parcours.h"
#include "structure.h"
#include "TriFusion.h"
#include "utiles.h"

listeCycles* baseDeCyclesMinimale(grapheMol g);
int** matriceAretesDansCycles(grapheMol g, listeCycles *cycles, int *nb_aretes);
listeCycles* eliminationGaussienne(grapheMol g, listeCycles *cycles);

#endif
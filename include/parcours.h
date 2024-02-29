#ifndef PARCOURS_H
#define PARCOURS_H

#include<stdio.h>
#include<stdlib.h>

#include "structure.h"
#include "Horton.h"
#include "utiles.h"

void parcoursEnLargeur(grapheMol g, listeCycles *cycles, int i, int *parents);
cycle convertirEnCycle(int v_id, int x_id, int x_niv, int y_id, int y_niv, int *parents);
int disjointsEtOrdonnes(int v, int x, int x_etage, int y, int y_etage, int *parents);

#endif
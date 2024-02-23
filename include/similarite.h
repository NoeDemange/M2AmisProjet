#ifndef SIMILARITE_H
#define SIMILARITE_H

#include "structure.h"
#include "clique.h"
#include "utiles.h"

float similarite(grapheCycles g1, grapheCycles g2);
int*  grapheCommunG12(grapheCycles g1, grapheCycles g2, couple *sommets, int taille, int* clique_max);
grapheSim* produitGraphesCycles(grapheCycles g1, grapheCycles g2, couple *sommets, int taille);
couple* couplesCyclesCompatibles(grapheCycles g1, grapheCycles g2, int *taille);
float restrictionTailleCycles(int taille1, int taille2);

#endif
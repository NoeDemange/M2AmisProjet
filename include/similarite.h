#ifndef SIMILARITE_H
#define SIMILARITE_H

#include "structure.h"
#include "clique.h"
#include "utiles.h"

int absolue(int a);
couple *couplesCyclesCompatibles(grapheCycles g1, grapheCycles g2, int *taille);
typeArete ** matriceAdjacence(grapheCycles g1);
grapheSim* produitGraphesCycles(grapheCycles g1, grapheCycles g2);
int*  grapheCommunG12(int taille, int* clique_max, grapheCycles g1, grapheCycles g2);
float similarite(grapheCycles g1, grapheCycles g2);

#endif
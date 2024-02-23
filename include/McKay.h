#ifndef MCKAY_H
#define MCKAY_H

#include "structure.h"
#include "utiles.h"

int* numerotationCanonique(grapheMol *graphe_mol);
void passageTableauPerm(grapheMol *graphe_mol, int *perm, int premier, int *temp1, int *temp2);
void grapheCanonique(grapheMol *graphe_mol);

#endif
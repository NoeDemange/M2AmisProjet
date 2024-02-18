#ifndef TRIFUSION_H
#define TRIFUSION_H

#include "structure.h"

void merge(Cycle cycles[], int l, int m, int r);
void mergeSort(Cycle cycles[], int l, int r);
void triFusion(Cycle cycles[], int taille);

void triParInsertionSommets(Cycle c, int id_cycle, indexCycles *index_cycles);

#endif
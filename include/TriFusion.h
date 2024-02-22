#ifndef TRIFUSION_H
#define TRIFUSION_H

#include <stdio.h>
#include <stdlib.h>

#include "structure.h"

void merge(cycle cycles[], int l, int m, int r);
void mergeSort(cycle cycles[], int l, int r);
void triFusion(cycle cycles[], int taille);

void triParInsertionSommets(cycle c);

#endif
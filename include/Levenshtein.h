#ifndef LEVENSHTEIN_H
#define LEVENSHTEIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utiles.h"

float distLevenshteinNormalise(grapheCycles g1, grapheCycles g2, int **matrice);
int distanceLevenshtein(char **mol1, int taille1, char **mol2, int taille2, int **matrice);

#endif


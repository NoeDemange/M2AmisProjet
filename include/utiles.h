#ifndef UTILES_H
#define UTILES_H

#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/stat.h>

#include "structure.h"

double chrono();
void tempsExecution(double sec, char *info);
void verifScan(int valeur, char *nom_fichier);
void printTab(int *tab, int n);
void printMatrice(int **matrice, int n, int m);
void generate_dot_file(grapheCycles *graph);

#endif
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
int absolue(int a);
int min( int a , int b);
int minTrois(int a, int b, int c);
void printTab(int *tab, int n);
void printMatrice(int **matrice, int n, int m);
void genererFichierDot(grapheCycles *graph);
void ecrireMatriceDansCSV(int n, float **matrix,grapheCycles *liste_GC, const char* filename);

#endif
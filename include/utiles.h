#ifndef UTILES_H
#define UTILES_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <sys/stat.h>

#include "structure.h"


void* allouer(size_t taille, char *message);
void* callouer(size_t nb_el, size_t taille, char *message);
void* reallouer(void *ptr, size_t taille, char *message);
void scannerFichier(int valeur, char *nom_fichier);
FILE* ouvrirFichier(const char *nom_fichier, const char *mode);

char* allouerChaine(char *chaine);
void creerDossier(const char *chemin);

double chrono();
void tempsExecution(double sec, char *info);

void printTab(int *tab, int n);
void printMatrice(int **matrice, int n, int m);

#endif
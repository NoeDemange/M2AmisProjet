#ifndef FICHIERS_H
#define FICHIERS_H

#include "structure.h"

void test(char *nom_dossier, char *nom_fichier, char *nom_fichier1);
void procedure(char *nom_dossier, int max_fichiers);
listeFichiers* lireDossier(char *nom_dossier, int max_fichiers, int *max_sommets, int *iter);
grapheMol lireFichier(char* nom_dossier, char *nom_fichier);

#endif
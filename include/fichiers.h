#ifndef FICHIERS_H
#define FICHIERS_H

#include "structure.h"

void test(char *nom_dossier, char *nom_fichier);
void procedure(char *nom_dossier, int max_fichiers);
listeFichiers* lireDossier(char *nom_dossier, int max_fichiers, int *max_sommets);
grapheMol lireFichier(char* nom_dossier, char *nom_fichier);
char* trouverNomFichier(char* nom_dossier, char *chebi_id);
char* allouerChaine(char *chaine);

#endif
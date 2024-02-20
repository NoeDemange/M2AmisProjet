#ifndef FICHIERS_H
#define FICHIERS_H

#include "structure.h"

void procedure(char *nom_dossier, int max_fichiers);
listeFichiers* lireDossier(char *nom_dossier, int max_fichiers);
grapheMol lireFichier(char* nom_dossier, char *nom_fichier);

#endif
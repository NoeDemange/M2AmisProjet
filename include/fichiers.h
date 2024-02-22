#ifndef FICHIERS_H
#define FICHIERS_H

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "structure.h"
#include "utiles.h"
#include "Horton.h"
#include "McKay.h"
#include "grapheCycles.h"
#include "parcours.h"
#include "similarite.h"

void test(char *nom_dossier, char *chebi_id, char *chebi_id1);
void procedure(char *nom_dossier, int max_fichiers);
listeFichiers* lireDossier(char *nom_dossier, int max_fichiers, int *max_sommets, int *iter);
grapheMol lireFichier(char* nom_dossier, char *nom_fichier);
char* trouverNomFichier(char* nom_dossier, char *chebi_id);
char* allouerChaine(char *chaine);

#endif
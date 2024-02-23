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
#include "Levenshtein.h"

void comparaison(char *nom_dossier, char *chebi_id, char *chebi_id1, int dot_option);
void procedure(char *nom_dossier, int max_fichiers, char *chebi_id);
listeFichiers* lireDossier(char *nom_dossier, int max_fichiers, int *max_sommets, int *iter);
grapheMol lireFichier(char* nom_dossier, char *nom_fichier, int opt);
char* trouverNomFichier(char* nom_dossier, char *chebi_id);
void genererFichierDotGC(grapheCycles *graph);
void genererFichierDotGM(grapheMol *graph); 
void genererFichierDotGP(grapheSim *g, int id1, int id2);
void ecrireMatriceDansCSV(int n, float **matrix,grapheCycles *liste_GC, const char* filename);
void ecrireTableauDansCSV(int n, float *tableau, grapheCycles *liste_GC, const char *nom_fichier);

#endif
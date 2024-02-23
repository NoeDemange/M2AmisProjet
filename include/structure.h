#ifndef STRUCTURE_H
#define STRUCTURE_H

#include <stdio.h>
#include <stdlib.h>

// Définie la taille max du buffer contenant l'id des cycles d'un sommet.
// En effet, un buffer indexCycles* est défini, dans lequel on indexe l'id des cycles
// auxquels appartient un sommet i.
#define SIZE_INDEX 20

//similarité
#define AUCUNE_LIAISON (-1024)
#define date 50

//#define TEST

#define OPTSTR "a:b:n:g:h"
#define USAGE_FMT  "Calcul de similarité des molécules issues de la base données ChEBI par comparaison de leur graphe de cycles.\n\n\
Usage : [-a chebi_id1 (défaut : vide)] [-b chebi_id2 (défaut : vide)] \
[-n nb_fichiers (défaut : tous)] [-g (défaut : non)] [-h (usage)]\n\n\
 -n      nombre de molécules issues de la base à comparer. Toutes si non renseigné. \
Seulement les molécules chebi_id1 et chebi_id2 si -a et -b sont renseignés.\n\
 -a,-b   prennent uniquement le numéro de l'ID de la molécule. Si -a seulement, compare la molécule à n autres molécules.\n\
 -g      génère des fichiers DOT pour afficher le graphe des cycles des molécules si -a et -b sont renseignés.\n"

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define ABS(a) ((a) > 0 ? (a) : (-a))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MIN3(a, b, c) ((a) < (c) ? (a) : (b) < (c) ? (b) : (c))

typedef struct grapheMol {
  int chebi_id;
  char **types;
  int nb_sommets;
  int **adjacence;
} grapheMol;

typedef struct couple {
	int id1;
	int id2;
}couple;

typedef struct typeArete {
	int type;
	int poids;
}typeArete;

typedef struct sommet {
  int id;
  int taille;
} sommet;

typedef struct arete {
  int id1;
  int id2;
  int type;
  int poids;
} arete;

typedef struct grapheSim {
  int nb_sommets;
  int **adjacence;
} grapheSim;

typedef struct listeAretes {
  arete a;
  struct listeAretes *suiv;
} listeAretes;

typedef struct cycle {
  int source;
  int *sommets;
  int taille; 
} cycle;

typedef struct listeCycles {
  cycle *cycles;
  int nb_cycles;
} listeCycles;

typedef struct grapheCycles {
  int chebi_id;
  char **types;
  int nb_atomes;
  int nb_sommets;
  int nb_aretes;
  sommet *sommets;
  typeArete **types_aretes;
} grapheCycles;

typedef struct indexCycles {
  int *cycles;
  int taille;
} indexCycles;

typedef struct listeFichiers {
  char *nom;
  struct listeFichiers *suiv;
} listeFichiers;

typedef struct element {
    int id;
    struct element *suiv;
} element; 

typedef struct file {
    element *tete;
    element *queue;
} file;

typedef struct options {
  char *chebi_id1;
  char *chebi_id2;
  int nb_fichiers;
  int graphe;
} options;

grapheMol initGrapheMol(int nb_sommets, int chebi_id);
void resetGrapheMol(grapheMol g);
void freeGrapheMol(grapheMol g);
void printGrapheMol(grapheMol g);

listeFichiers* initListeFichiers(void);
listeFichiers* freeListeFichiers(listeFichiers *fichier);
void ajouterNomFichier(listeFichiers **fichiers, char *nom_fichier);
void printListeFichiers(listeFichiers *fichiers);

int estVide(file *file);
element* initElement(int id);
file* initFile(int id);
void ajouterDansFile(file *file, int id);
element defiler(file *file);
void printFile(file *file);

listeCycles* initListeCycles();
void ajouterCycleDansListe(listeCycles *cycles, cycle c);
void freeListeCycles(listeCycles *cycles);
void freeTousListeCycles(listeCycles *cycles);
void printListeCycles(listeCycles *cycles);

indexCycles* initIndexCycles(int taille);
void resetIndexCycles(indexCycles *index_cycles, int taille);
void ajouterCycleDansIndex(indexCycles *index_cycles, int id_sommet, int id_cycle);
void freeIndexCycles(indexCycles *index_cycles);
void printIndexCycles(indexCycles *index_cycles, int taille);

grapheCycles initGrapheCycles(listeCycles *cycles, int chebi_id, int nb_atomes, char **types);
arete initArete(int id1, int id2, int type, int poids);
void ajouterAreteDansListe(listeAretes **aretes, int *nb_aretes, int id1, int id2, int type, int poids);
void ajouterAreteDansGraphe(grapheCycles *g, listeAretes *aretes, int nb_aretes);
void freeListeAretes(listeAretes *aretes);
void freeGrapheCycles(grapheCycles g);
void printGrapheCycles(grapheCycles g);

#endif
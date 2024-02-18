#ifndef STRUCTURE_H
#define STRUCTURE_H

// Définie la taille max du buffer contenant l'id des cycles d'un sommet.
// En effet, un buffer indexCycles* est défini, dans lequel on indexe l'id des cycles
// auxquels appartient un sommet i.
#define SIZE_INDEX 20

typedef struct grapheMol {
  int chebi_id;
  int nb_sommets;
  int **adjacence;
} grapheMol;

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

typedef struct listeAretes {
  arete a;
  struct listeAretes *suiv;
} listeAretes;

typedef struct Cycle {
  int source;
  int* sommets;
  int taille; 
} Cycle;

typedef struct listeCycles {
  Cycle *cycles;
  int nb_cycles;
} listeCycles;

typedef struct grapheCycles {
  int chebi_id;
  int nb_sommets;
  int nb_aretes;
  sommet *sommets;
  arete *aretes;
} grapheCycles;

typedef struct indexCycles {
  int *cycles;
  int taille;
} indexCycles;

typedef struct listeFichiers {
  char *nom;
  struct listeFichiers *suiv;
} listeFichiers;

void procedure(char *nom_dossier, int max_fichiers);
listeFichiers* lireDossier(char *nom_dossier, int max_fichiers);
grapheMol lireFichier(char* nom_dossier, char *nom_fichier);

grapheMol initGrapheMol(int nb_sommets, int chebi_id);
void freeGrapheMol(grapheMol g);
void printGrapheMol(grapheMol g);

listeFichiers* initListeFichiers(void);
listeFichiers* freeListeFichiers(listeFichiers *fichier);
void ajouterNomFichier(listeFichiers **fichiers, char *nom_fichier);
void printListeFichiers(listeFichiers *fichiers);

indexCycles* initIndexCycles(int taille);
void resetIndexCycles(indexCycles *index_cycles, int taille);
void ajouterCycleDansIndex(indexCycles *index_cycles, int id_sommet, int id_cycle);
void freeIndexCycles(indexCycles *index_cycles);
void printIndexCycles(indexCycles *index_cycles, int taille);

grapheCycles initGrapheCycles(listeCycles *liste_c);
arete initArete(int id1, int id2, int type, int poids);
void ajouterAreteDansListe(listeAretes **aretes, int *nb_aretes, int id1, int id2, int type, int poids);
void ajouterAreteDansGraphe(grapheCycles *g, listeAretes *aretes, int nb_aretes);
void freeListeAretes(listeAretes *aretes);
void freeGrapheCycles(grapheCycles g);
void printGrapheCycles(grapheCycles g);

#endif
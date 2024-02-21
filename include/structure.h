#ifndef STRUCTURE_H
#define STRUCTURE_H

// Définie la taille max du buffer contenant l'id des cycles d'un sommet.
// En effet, un buffer indexCycles* est défini, dans lequel on indexe l'id des cycles
// auxquels appartient un sommet i.
#define SIZE_INDEX 20

#define TEST

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

typedef struct cycle {
  int source;
  int* sommets;
  int taille; 
} cycle;

typedef struct listeCycles {
  cycle *cycles;
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

typedef struct element {
    int id;
    struct element *suiv;
} element; 

typedef struct file {
    element *tete;
    element *queue;
} file;

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

cycle *initCycle();
listeCycles* initListeCycles();
void ajouterCycleDansListe(listeCycles *cycles, cycle c);
void freeListeCycles(listeCycles *cycles);
void printListeCycles(listeCycles *cycles);

indexCycles* initIndexCycles(int taille);
void resetIndexCycles(indexCycles *index_cycles, int taille);
void ajouterCycleDansIndex(indexCycles *index_cycles, int id_sommet, int id_cycle);
void freeIndexCycles(indexCycles *index_cycles);
void printIndexCycles(indexCycles *index_cycles, int taille);

grapheCycles initGrapheCycles(int chebi_id, listeCycles *liste_c);
arete initArete(int id1, int id2, int type, int poids);
void ajouterAreteDansListe(listeAretes **aretes, int *nb_aretes, int id1, int id2, int type, int poids);
void ajouterAreteDansGraphe(grapheCycles *g, listeAretes *aretes, int nb_aretes);
void freeListeAretes(listeAretes *aretes);
void freeGrapheCycles(grapheCycles g);
void printGrapheCycles(grapheCycles g);

#endif
#ifndef STRUCTURE_H
#define STRUCTURE_H

typedef struct graphmol {
  int chebi_id;
  int nb_sommets;
  int **adjacence;
} graphmol;

typedef struct filenames {
  char *nom;
  struct filenames *suiv;
} filenames;

void procedure(char *nom_dossier, int max_fichiers);
filenames* lireDossier(char *nom_dossier, int max_fichiers);
graphmol lireFichier(char* nom_dossier, char *nom_fichier);

graphmol initGraphMol(int nb_sommets, int chebi_id);
void freeGraphMol(graphmol g);
void printGraphMol(graphmol g);

filenames* initFileNames(void);
filenames* freeFileName(filenames *fichier);
void addFileName(filenames **fichiers, char *nom_fichier);
void printFileNames(filenames *fichiers);

#endif
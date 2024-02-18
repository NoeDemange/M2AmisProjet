#ifndef STRUCTURE_H
#define STRUCTURE_H

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

typedef struct grapheCycle {
  int chebi_id;
  int nb_sommets;
  int nb_aretes;
  sommet *sommets;
  arete *aretes;
} grapheCycle;

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

#endif
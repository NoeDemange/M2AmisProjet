#include "Levenshtein.h"

// Distance de Levenshtein appliquée aux chaînes de caractères
// décrivant les atomes d'une molécule.
// Normalisée entre 0 et 1 et modifiée pour que 1 corresponde à l'identité.
float distLevenshteinNormalise(grapheCycles g1, grapheCycles g2, int **matrice) {

  float lev = distanceLevenshtein(g1.types, g1.nb_atomes, g2.types, g2.nb_atomes, matrice);
  return 1 - (lev / MAX(g1.nb_atomes, g2.nb_atomes));
}

// Distance de Levenshtein entre deux chaînes de caractères.
// La matrice doit être instanciée avec deux lignes d'une taille
// supérieure au max des 2 tailles + 1. Si NULL, instanciée dans la fonction.
int distanceLevenshtein(char *mol1, int taille1, char *mol2, int taille2, int **matrice) {

  char *chaine1, *chaine2;
  int *ligne_prec;
  int *ligne_cour;
  int i, j;
  int cout, distance;

  // Chaque car de la chaine1 est comparé aux car de la chaine2
  // On choisit chaine2 comme étant la plus grande chaîne
  if (taille1 <= taille2) {
    chaine1 = mol1;
    chaine2 = mol2;
  }
  else {
    chaine1 = mol2;
    chaine2 = mol1;
    int temp = taille1;
    taille1 = taille2;
    taille2 = temp;
  }

  if (matrice == NULL) {
    ligne_prec = allouer((taille2 + 1) * sizeof(int), "première ligne de la matrice des distances de Levenshtein (Levenshtein.c)");
    ligne_cour = allouer((taille2 + 1) * sizeof(int), "deuxième ligne de la matrice des distances de Levenshtein (Levenshtein.c)");
  }
  else {
    ligne_prec = matrice[0];
    ligne_cour = matrice[1];
  }

  // Initialisation
  for (j = 0; j < taille2 + 1; j++) {
    ligne_prec[j] = j;
  }

  for (i = 1; i < taille1 + 1; i++) {
    // Calcul de la distance
    ligne_cour[0] = i;
    for (j = 1; j < taille2 + 1; j++) {
      cout = (chaine1[i - 1] == chaine2[j - 1]) ? 0 : 1;
      ligne_cour[j] = MIN3(ligne_prec[j] + 1, 
                          ligne_cour[j - 1] + 1, 
                          ligne_prec[j - 1] + cout);
    }
    // Ligne précédente devient ligne courante
    for (j = 0; j < taille2 + 1; j++) {
      ligne_prec[j] = ligne_cour[j];
    }
  }

  distance = ligne_cour[taille2];

  if (matrice == NULL) {
    free(ligne_prec);
    free(ligne_cour);
  }

  return distance;
}
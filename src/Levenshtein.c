#include "Levenshtein.h"

int distanceLevenshtein(char *mol1, int taille1, char *mol2, int taille2) {

  char *chaine1, *chaine2;
  int *ligne_prec, *ligne_cour;
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
  // TODO En faire des buffers de taille max_nb_sommets...
  ligne_prec = malloc((taille2 + 1) * sizeof(int));
  ligne_cour = malloc((taille2 + 1) * sizeof(int));

  // Initialisation
  for (j = 0; j < taille2 + 1; j++) {
    ligne_prec[j] = j;
  }

  for (i = 1; i < taille1 + 1; i++) {
    // Calcul de la distance
    ligne_cour[0] = i;
    for (j = 1; j < taille2 + 1; j++) {
      cout = (chaine1[i - 1] == chaine2[j - 1]) ? 0 : 1;
      ligne_cour[j] = minTrois(ligne_prec[j] + 1, 
                          ligne_cour[j - 1] + 1, 
                          ligne_prec[j - 1] + cout);
    }
    // Ligne précédente devient ligne courante
    for (j = 0; j < taille2 + 1; j++) {
      ligne_prec[j] = ligne_cour[j];
    }
  }

  distance = ligne_cour[taille2];

  free(ligne_prec);
  free(ligne_cour);

  return distance;
}
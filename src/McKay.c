#include "McKay.h"
#include "nauty.h"

int* numerotationCanonique(grapheMol *graphe_mol) {

  // Déclaration des pointeurs sans allouer de mémoire.
  DYNALLSTAT(graph,g,g_sz);
  DYNALLSTAT(graph,canong,canong_sz);
  DYNALLSTAT(int,lab,lab_sz);
  DYNALLSTAT(int,ptn,ptn_sz);
  DYNALLSTAT(int,orbits,orbits_sz);

  // Définition des options
  static DEFAULTOPTIONS_GRAPH(options);
  options.getcanon = TRUE;
  statsblk stats;
  
  int n,m,i,j;

  n = graphe_mol->nb_sommets;
  m = SETWORDSNEEDED(n);

  // Vérifie la compatibilité des procédures de nauty
  nauty_check(WORDSIZE,m,n,NAUTYVERSIONID);

  // Allocation mémoire
  DYNALLOC2(graph,g,g_sz,m,n,"malloc");
  DYNALLOC2(graph,canong,canong_sz,n,m,"malloc");
  DYNALLOC1(int,lab,lab_sz,n,"malloc");
  DYNALLOC1(int,ptn,ptn_sz,n,"malloc");
  DYNALLOC1(int,orbits,orbits_sz,n,"malloc");

  EMPTYGRAPH(g,m,n);

  for (i = 0; i < n; i++) {
    for (j = i + 1; j < n; j++) {
      if (graphe_mol->adjacence[i][j] == 1) {
        ADDONEEDGE(g,i,j,m);
      }
    }
  }

  densenauty(g,lab,ptn,orbits,&options,&stats,m,n,canong);

  int *perm = allouer(n * sizeof(int), "tableau des permutations des sommets d'un graphe (McKay.c)");

  for (i = 0; i < n; i++) {
    perm[lab[i]] = i;
  }

  return perm;
}

void passageTableauPerm(grapheMol *graphe_mol, int *perm, int premier, int *temp1, int *temp2) {

  int n = graphe_mol->nb_sommets;
  int temp;
  int i, j;
  char *c1, *c2;

  // Permutation des lignes
  // Première ligne permutée
  for (j = 0; j < n; j++) {
    temp1[j] = graphe_mol->adjacence[perm[premier]][j];
    graphe_mol->adjacence[perm[premier]][j] = graphe_mol->adjacence[premier][j];
  }
  // Permutation des atomes (caractères)
  c1 = graphe_mol->types[perm[premier]];
  graphe_mol->types[perm[premier]] = graphe_mol->types[premier]; 

  i = perm[premier];
  // Toutes les autres lignes
  while (i != premier) {
    for (j = 0; j < n; j++) {
      temp2[j] = graphe_mol->adjacence[perm[i]][j];
      graphe_mol->adjacence[perm[i]][j] = temp1[j];
      temp1[j] = temp2[j];
    }
    c2 = graphe_mol->types[perm[i]];
    graphe_mol->types[perm[i]] = c1;
    c1 = c2;
    i = perm[i];
  }

  // Permutation des colonnes
  // Première colonne permutée
  for (i = 0; i < n; i++) {
    temp1[i] = graphe_mol->adjacence[i][perm[premier]];
    graphe_mol->adjacence[i][perm[premier]] = graphe_mol->adjacence[i][premier];
  }

  j = perm[premier];
  perm[premier] = premier;
  // Toutes les autres colonnes
  while (j != premier) {
    for (i = 0; i < n; i++) {
      temp2[i] = graphe_mol->adjacence[i][perm[j]];
      graphe_mol->adjacence[i][perm[j]] = temp1[i];
      temp1[i] = temp2[i];
    }
    temp = j;
    j = perm[j];
    perm[temp] = temp;
  }
}

void grapheCanonique(grapheMol *graphe_mol) {

  int n = graphe_mol->nb_sommets;
  int i, premier = -1, pas_de_changement;

  int *perm = numerotationCanonique(graphe_mol);

  // Trouver le premier sommet à permuter
  for (i = 0; i < n; i++) {
    if (perm[i] != i) {
      premier = i;
      break;
    }
  }
  // Déjà canonique
  if (premier == -1) {
    free(perm);
    return;
  }

  int *temp1 = allouer(n * sizeof(int), "tableau temporaire 1 de permutation des sommets (McKay.c)");
  int *temp2 = allouer(n * sizeof(int), "tableau temporaire 2 de permutation des sommets (McKay.c)");

  while (premier != -1) {

    passageTableauPerm(graphe_mol, perm, premier, temp1, temp2);
    
    pas_de_changement = 1;
    // Trouver le prochain sommet à permuter
    for (i = 0; i < n; i++) {
      if (perm[i] != i) {
        premier = i;
        pas_de_changement = 0;
        break;
      }
    }
    // Tous les sommets sont permutés
    if (pas_de_changement) {
      premier = -1;
    }
  }

  free(perm);
  free(temp1);
  free(temp2);
}
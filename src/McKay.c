#include "nauty.h"

void testMcKay(void) {

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
  
  int n,m,i;

  n = 7;
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
  ADDONEEDGE(g,0,1,m);
  ADDONEEDGE(g,1,2,m);
  ADDONEEDGE(g,2,3,m);
  ADDONEEDGE(g,3,4,m);
  ADDONEEDGE(g,4,5,m);
  ADDONEEDGE(g,5,6,m);
  ADDONEEDGE(g,6,0,m);
  ADDONEEDGE(g,2,4,m);
  ADDONEEDGE(g,1,5,m);

  printf("Numérotation canonique d'un graphe de %d sommets :\n",n);
  densenauty(g,lab,ptn,orbits,&options,&stats,m,n,canong);

  for (i = 0; i < n; i++) {
    printf(" %d->%d", lab[i] + 1, i + 1);
  }
  printf("\n");
}
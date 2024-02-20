#ifndef PARCOURS_H
#define PARCOURS_H

listeCycles* baseDeCyclesMinimale(grapheMol g);
int** matriceAretesDansCycles(grapheMol g, listeCycles *cycles, int *nb_aretes);
void parcoursEnLargueur(grapheMol g, listeCycles *cycles, int i, int *parents);
int disjointsEtOrdonnes(int v, int x, int x_etage, int y, int y_etage, int *parents);

#endif
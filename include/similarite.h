#ifndef SIMILARITE_H
#define SIMILARITE_H

#include "structure.h"
//#include "grapheCycles.h"
#include "graph.h"
#include "clique.h"

int valeur_absolue(int a);
couple *construction_couples_cycles(grapheCycles a,grapheCycles b,int taille);
type_arete ** construction_matrice_graphe_cycles(grapheCycles a);
void liberer_type_arete(type_arete **m, grapheCycles a);
graph graphe_produit_cycles(grapheCycles a,grapheCycles b);
int*  graphe_g12_cycles(graph g12, int* clique_max,grapheCycles a,grapheCycles b);
float similarite(grapheCycles a,grapheCycles b);

#endif
#ifndef CLIQUE_H
#define CLIQUE_H

#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>

#include "structure.h"
#include "utiles.h"

/**

* Bibliotheque d'aide pour le calcul d'une clique d'un graphe

* @author Dimitri Watel

*/

/**
* Renvoie une clique maximum de g.
* @param g : un graphe
* @param timeout : temps après lequel le calcul s'arrête, on renvoie alors la meilleure clique trouvée
* @return une clique maximum de g sous la forme d'un tableau de n entiers, où n est le nombre de noeuds de g. La valeur de cl[i] est 1 si le noeud i appartient à la clique et 0 sinon.
*/
int* cliqueMax(grapheSim *g, long timeout);

#endif
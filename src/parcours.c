#include<stdio.h>
#include<stdlib.h>

#include "structure.h"
#include "parcours.h"
#include "Horton.h"
#include "utiles.h"

listeCycles* baseDeCyclesMinimale(grapheMol g) {

    int i;
    listeCycles *cycles = initListeCycles();
    listeCycles *base_minimale;
    int *parents = malloc(g.nb_sommets * sizeof(int));

    for (i = 0; i < g.nb_sommets; i++) {
        parcoursEnLargueur(g, cycles, i, parents);
    }
    free(parents);
    triFusion(cycles->cycles, cycles->nb_cycles);

    #ifdef TEST
        printListeCycles(cycles);
    #endif

    base_minimale = eliminationGaussienne(g, cycles);

    return base_minimale;
}

int** matriceAretesDansCycles(grapheMol g, listeCycles *cycles, int *nb_aretes) {

    int i,j;
    int id1, id2, arete_id;
    *nb_aretes = 0;
    int **matrice_aretes = calloc(cycles->nb_cycles, sizeof(int*));    

    // Numéroter les arêtes dans la matrice d'adjacence
    for (i = 0; i < g.nb_sommets; i++) {
        for (j = i + 1; j < g.nb_sommets; j++) {
            if (g.adjacence[j][i]) {
                g.adjacence[j][i] += (*nb_aretes)++;
            }
        }
    }
    // Mémoire
    for (i = 0; i < cycles->nb_cycles; i++) {
        matrice_aretes[i] = calloc(*nb_aretes, sizeof(int)); 
    }
    // Remplir la matrice matrice_aretes
    for (i = 0; i < cycles->nb_cycles; i++) {
        // Mémoire
        // Arête entre premier et dernier atome du cycle
        id1 = cycles->cycles[i].sommets[0];
        id2 = cycles->cycles[i].sommets[cycles->cycles[i].taille - 1];

        if (id1 < id2) {
            arete_id = g.adjacence[id2][id1] - 1;
        }
        else {
            arete_id = g.adjacence[id1][id2] - 1;
        }
        matrice_aretes[i][arete_id] = 1;

        // Arête entre si et si+1 dans le cycle
        for (j = 0; j < cycles->cycles[i].taille - 1; j++) {
            id1 = cycles->cycles[i].sommets[j];
            id2 = cycles->cycles[i].sommets[j + 1];

            if (id1 < id2) {
                arete_id = g.adjacence[id2][id1] - 1;
            }
            else {
                arete_id = g.adjacence[id1][id2] - 1;
            }
            matrice_aretes[i][arete_id] = 1;
        }
    }
    return matrice_aretes;
}

listeCycles* eliminationGaussienne(grapheMol g, listeCycles *cycles) {
    
    int i, j, k, premier;
    int somme_ligne;
    int nb_aretes;
    int **matrice_aretes = matriceAretesDansCycles(g, cycles, &nb_aretes);
    
    // printMatrice(matrice_aretes, cycles->nb_cycles, *nb_aretes);
    
    int *cycles_independants = malloc(cycles->nb_cycles * sizeof(int));
    int *cycles_deja_ajoutes =  calloc(cycles->nb_cycles, sizeof(int));
    listeCycles *base_minimale = initListeCycles();

    for (i = 0; i < cycles->nb_cycles; i++) {
        cycles_independants[i] = 2;
    }

    #ifdef TEST
        printMatrice(matrice_aretes, cycles->nb_cycles, nb_aretes);
    #endif
   
    // Elimination de Gauss
    for (i = 0; i < cycles->nb_cycles - 1; i++) {

        premier = -1;
        for (j = 0; j < nb_aretes; j++) {
            if(matrice_aretes[i][j] == 1) {
                premier = j;
                break;
            }
        }
        if (premier != -1) {
            for (j = i + 1; j < cycles->nb_cycles; j++) {
                if (matrice_aretes[j][premier] == 1) {
                    somme_ligne = 0;
                    for (k = 0; k < nb_aretes; k++) {
                        matrice_aretes[j][k] ^=  matrice_aretes[i][k];// XOR
                        if (matrice_aretes[j][k]) {
                            somme_ligne++;
                        }
                    }
                    cycles_independants[j] = (somme_ligne > 0) ? 1 : 0;

                    #ifdef TEST
                        printf("%d <- %d XOR %d\n", j, j, i);
                        printMatrice(matrice_aretes, cycles->nb_cycles, nb_aretes);
                    #endif
                }
            }
        }
    }

    #ifdef TEST
        printMatrice(matrice_aretes, cycles->nb_cycles, nb_aretes);
    #endif

    // Cycles indépendants
    for (i = 0; i < cycles->nb_cycles; i++) {
        if (cycles_independants[i]) {
            ajouterCycleDansListe(base_minimale, cycles->cycles[i]);
        }
    }

    // Libérer la mémoire
    for (int i = 0; i < cycles->nb_cycles; i++) {
        free(matrice_aretes[i]);
    }
    free(matrice_aretes);
    free(cycles_independants);
    free(cycles_deja_ajoutes);
    freeListeCycles(cycles);
    resetGrapheMol(g);

    return base_minimale;
}

// Transforme un sommet v et une arête x-y en cycle, en ordonnant
// les sommets : v - sommets de v à x - y - sommet de y à v.
// La taille du cycle est égale à la sommes des niveaux de x et y
// dans l'arbre de parcours à partir de v + 1 (pour le sommet v).
// On obtient la chaîne entre v et x en parcourant la liste des
// prédecesseurs dans parents.
void convertirEnCycle(cycle *c, int v_id, int x_id, int x_niv, int y_id, int y_niv, int *parents) {

    int i;
    c->source = v_id;
    c->taille = x_niv + y_niv + 1;
    c->sommets = malloc(c->taille * sizeof(int));
    c->sommets[0] = v_id;
    
    // Ecrire les sommets entre 1 et x_niv, en partant de la fin
    // = sommets de v à x
    for (i = x_niv; i > 0; i--) {
        c->sommets[i] = x_id;
        x_id = parents[x_id];
    }
    // Ecrire les sommets entre x_niv + 1 et la fin du cycle, en partant du début
    // = sommet de y à v
    for (i = x_niv + 1; i < x_niv + y_niv + 1; i++) {
        c->sommets[i] = y_id;
        y_id = parents[y_id];
    }
}


void parcoursEnLargueur(grapheMol g, listeCycles *cycles, int i, int *parents) {
    
    int j, id1, id2;
    element tete;
    cycle *c;
    int niveau, nb_restant_dans_niveau, nb_nouv_dans_niveau_suivant; 
    int *visites = calloc(g.nb_sommets, sizeof(int));
    int *niveaux = malloc(g.nb_sommets * sizeof(int));
    for (int i = 0; i < g.nb_sommets; i++) {
        parents[i] = -1;
    }

    niveau = 0;
    niveaux[i] = niveau;

    file *file = initFile(i);
    // printf("%d(%d) : ", i, niveau);

    nb_restant_dans_niveau = 1; // Nombre de sommets au niveau courant (à défiler) 
    nb_nouv_dans_niveau_suivant = 0; // Nombre de sommets au niveau suivant (à enfiler)

    while (!estVide(file)) {

        tete = defiler(file);

        visites[tete.id] = 1;

        for (j = 0; j < g.nb_sommets; j++) {
            if (tete.id < j) {
                id1 = tete.id;
                id2 = j;
            }
            else {
                id1 = j;
                id2 = tete.id;
            }
            // On ne regarde que les arêtes qui n'ont pas déjà été regardées
            if (g.adjacence[id2][id1] != -1 && g.adjacence[id1][id2] == 1) {
                if(visites[j] == 0) {
                    niveaux[j] = niveau + 1;
                    parents[j] = tete.id;
                    visites[j] = 1;
                    // printf("%d(%d)\n", j, niveaux[j]);
                    ajouterDansFile(file, j);
                    nb_nouv_dans_niveau_suivant++;
                }
                // Sommet j déjà atteint : si j != prédecesseur de tete.id-j, alors ils ferment un cycle
                else if (parents[tete.id] != j) {
                    // Marquer l'arête comme vue
                    g.adjacence[id2][id1] = -1;
                    if (disjointsEtOrdonnes(i, tete.id, niveaux[tete.id], j, niveaux[j], parents)) {
                        c = initCycle();
                        convertirEnCycle(c, i, tete.id, niveaux[tete.id], j, niveaux[j], parents);
                        ajouterCycleDansListe(cycles, *c);
                    }
                }
            }
        }
        if (--nb_restant_dans_niveau == 0) {
            niveau++;
            nb_restant_dans_niveau = nb_nouv_dans_niveau_suivant;
            nb_nouv_dans_niveau_suivant = 0;
        }
    }
    free(file);
    free(visites);
    free(niveaux);
    resetGrapheMol(g);
}

// Teste si le sommet v et l'arête x et y forme un cycle.
// Teste également si les sommets du cycle potentiel ont
// un numéro inférieur à celui de v, pour ne séléctionner
// qu'une seule fois un même cycle.
// La chaîne la plus courte de v à x et de v à y est obtenue
// par un parcours en largeur (prédecesseurs stockés dans parents).
// x et y forme une arête, donc la taille du chemin qui les séparent à v
// est différente d'au plus une arête. Pour comparer les deux
// chemins, on peut comparer les sommets de chaque chemin qui
// sont situés au même niveau dans l'arbre du parcours.
int disjointsEtOrdonnes(int v_id, int x_id, int x_niv, int y_id, int y_niv, int *parents) {
    
    // Ordre par rapport à x et y
    if (v_id < x_id || v_id < y_id) {
        return 0;
    }

    int diff, chem1, chem2;

    if (x_niv > y_niv) {
        diff = x_niv - y_niv;
        chem1 = x_id;
        chem2 = y_id;
    }
    else {
        diff = y_niv - x_niv;
        chem1 = y_id;
        chem2 = x_id;
    }
    if (diff > 1) {
        printf("Mauvais usage de disjointsEtOrdonnes (parcours.c) : ");
        printf("les sommets x et y ne devraient pas être reliés par une arête.\n");
        exit(EXIT_FAILURE);
    }
    while (diff-- > 0 && chem1 != v_id) {
        chem1 = parents[chem1];
        // Teste ordre
        if (v_id < chem1) {
            return 0;
        }
    }
    while (chem1 != v_id) {
        chem1 = parents[chem1];
        chem2 = parents[chem2];
        // Intersection != {v} ou ordre non respecté
        if ((chem1 == chem2 && chem1 != v_id) || v_id < chem1 || v_id < chem2) {
            return 0;
        }
    }          
    return 1;
}


#include "Horton.h"

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
        // Libérer la mémoire
        else {
            if (cycles->cycles[i].sommets) {
                free(cycles->cycles[i].sommets);
            }
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
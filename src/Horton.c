#include "Horton.h"

listeCycles* baseDeCyclesMinimale(grapheMol g) {

    int i;
    listeCycles *cycles = initListeCycles();
    listeCycles *base_minimale;
    int *parents = allouer(g.nb_sommets * sizeof(int), "parents des sommets dans le parcours (Horton.c)");

    for (i = 0; i < g.nb_sommets; i++) {
        parcoursEnLargeur(g, cycles, i, parents);
    }
    free(parents);
    triFusion(cycles->cycles, cycles->nb_cycles);

    #ifdef TEST
        printListeCycles(cycles);
    #endif

    base_minimale = eliminationGaussienne(g, cycles);

    return base_minimale;
}

int** matriceIncidenceCycles(grapheMol g, listeCycles *cycles, int *nb_aretes) {

    int i,j;
    int id1, id2, arete_id;
    *nb_aretes = 0;
    int **incidence = callouer(cycles->nb_cycles, sizeof(int*), "matrice d'incidence des cycles (Horton.c)");    

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
        incidence[i] = callouer(*nb_aretes, sizeof(int), "matrice d'incidence des cycles (Horton.c)"); 
    }
    // Remplir la matrice incidence
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
        incidence[i][arete_id] = 1;

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
            incidence[i][arete_id] = 1;
        }
    }
    return incidence;
}

listeCycles* eliminationGaussienne(grapheMol g, listeCycles *cycles) {
    
    int i, j, k, premier;
    int somme_ligne;
    int nb_aretes;
    int **incidence = matriceIncidenceCycles(g, cycles, &nb_aretes);
    
    // printMatrice(incidence, cycles->nb_cycles, *nb_aretes);
    
    int *cycles_independants = allouer(cycles->nb_cycles * sizeof(int), "tableau binaire des cycles indépendants (Hortons.c)");
    int *cycles_deja_ajoutes =  callouer(cycles->nb_cycles, sizeof(int), "talbeau des cycles ajoutés dans la base (Horton.c)");
    listeCycles *base_minimale = initListeCycles();

    for (i = 0; i < cycles->nb_cycles; i++) {
        cycles_independants[i] = 2;
    }

    #ifdef TEST
       printMatrice(incidence, cycles->nb_cycles, nb_aretes);
    #endif
   
    // Elimination de Gauss
    for (i = 0; i < cycles->nb_cycles - 1; i++) {

        premier = -1;
        for (j = 0; j < nb_aretes; j++) {
            if(incidence[i][j] == 1) {
                premier = j;
                break;
            }
        }
        if (premier != -1) {
            for (j = i + 1; j < cycles->nb_cycles; j++) {
                if (incidence[j][premier] == 1) {
                    somme_ligne = 0;
                    for (k = 0; k < nb_aretes; k++) {
                        incidence[j][k] ^=  incidence[i][k];// XOR
                        if (incidence[j][k]) {
                            somme_ligne++;
                        }
                    }
                    cycles_independants[j] = (somme_ligne > 0) ? 1 : 0;

                    #ifdef TEST
                        printf("%d <- %d XOR %d\n", j, j, i);
                        printMatrice(incidence, cycles->nb_cycles, nb_aretes);
                    #endif
                }
            }
        }
    }

    #ifdef TEST
        printMatrice(incidence, cycles->nb_cycles, nb_aretes);
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
        free(incidence[i]);
    }
    free(incidence);
    free(cycles_independants);
    free(cycles_deja_ajoutes);
    freeListeCycles(cycles);
    resetGrapheMol(g);

    return base_minimale;
}
#include <stdio.h>
#include <stdlib.h>

#include "TriFusion.h"

void merge(cycle cycles[], int l, int m, int r) {
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    // Crée des tableaux temporaires
    cycle L[n1], R[n2];

    // Copie les données dans les tableaux temporaires L[] et R[]
    for (i = 0; i < n1; i++)
        L[i] = cycles[l + i];
    for (j = 0; j < n2; j++)
        R[j] = cycles[m + 1 + j];

    // Fusionne les tableaux temporaires en cycles[l..r]
    i = 0;
    j = 0;
    k = l;
    while (i < n1 && j < n2) {
        if (L[i].taille <= R[j].taille) {
            cycles[k] = L[i];
            i++;
        }
        else {
            cycles[k] = R[j];
            j++;
        }
        k++;
    }

    // Copie les éléments restants de L[], s'il y en a
    while (i < n1) {
        cycles[k] = L[i];
        i++;
        k++;
    }

    // Copie les éléments restants de R[], s'il y en a
    while (j < n2) {
        cycles[k] = R[j];
        j++;
        k++;
    }
}

// Tri fusion pour trier cycles[l..r] par taille croissante
void mergeSort(cycle cycles[], int l, int r) {
    if (l < r) {
        // Trouve le point médian
        int m = l + (r - l) / 2;

        // Trie d'abord les deux moitiés
        mergeSort(cycles, l, m);
        mergeSort(cycles, m + 1, r);

        // Fusionne les deux moitiés triées
        merge(cycles, l, m, r);
    }
}

// Fonction pour trier les cycles par taille croissante en utilisant le tri fusion
void triFusion(cycle cycles[], int taille) {
    mergeSort(cycles, 0, taille - 1);
}

void triParInsertionSommets(cycle c) {
    
    int *tab = c.sommets;
    int i, j, id_courant;

    for (i = 1; i < c.taille; i++) {
        id_courant = tab[i];
        j = i - 1;
 
        while (j >= 0 && tab[j] > id_courant) {
            tab[j + 1] = tab[j];
            j = j - 1;
        }
        tab[j + 1] = id_courant;
    }
}

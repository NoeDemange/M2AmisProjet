typedef struct {
    int source;
    int* parents;
    int taille; 
} Cycle;

void merge(Cycle cycles[], int l, int m, int r) {
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    // Crée des tableaux temporaires
    Cycle L[n1], R[n2];

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
void mergeSort(Cycle cycles[], int l, int r) {
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
void triFusion(Cycle cycles[], int taille) {
    mergeSort(cycles, 0, taille - 1);
}

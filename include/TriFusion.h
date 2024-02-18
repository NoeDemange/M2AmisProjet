#ifndef TRIFUSION_H
#define TRIFUSION_H

typedef struct {
    int source;
    int* sommets;
    int taille; 
} Cycle;

void merge(Cycle cycles[], int l, int m, int r);
void mergeSort(Cycle cycles[], int l, int r);
void triFusion(Cycle cycles[], int taille);

#endif
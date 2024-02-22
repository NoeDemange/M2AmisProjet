#include "structure.h"
#include "utiles.h"

grapheMol initGrapheMol(int nb_sommets, int chebi_id) {

  grapheMol g;
  g.nb_sommets = nb_sommets;
  g.chebi_id = chebi_id;
  g.types = NULL;
  g.adjacence = malloc(nb_sommets * sizeof(int*));

  for(int i = 0; i < nb_sommets; i++) {
    g.adjacence[i] = calloc(nb_sommets, sizeof(int));
  }

  return g;
}

void resetGrapheMol(grapheMol g) {

  int i,j;
  for (i = 0; i < g.nb_sommets; i++) {
    for (j = i + 1; j < g.nb_sommets; j++) {
      if (g.adjacence[j][i] < 0 || g.adjacence[j][i] > 1) {
        g.adjacence[j][i] = 1;
      }
    }
  }
}

void freeGrapheMol(grapheMol g) {

  if (g.adjacence) {
    for(int i = 0; i < g.nb_sommets; i++) {
      free(g.adjacence[i]);
    }
    free(g.adjacence);
  }
}

void printGrapheMol(grapheMol g) {
  
  printf("matrice adj de %d : %d\n", g.chebi_id, g.nb_sommets);
  for (int i = 0; i < g.nb_sommets; i++) {
    for (int j = 0; j < g.nb_sommets; j++) {
      printf(" %d", g.adjacence[i][j]);
    }
    printf("\n");
  }
  printf("%s\n", g.types);
  printf("\n");
}

listeFichiers* initListeFichiers(void) {

  listeFichiers *f = malloc(sizeof(struct listeFichiers));
  f->nom = NULL;
  f->suiv = NULL;
  return f;
}

void ajouterNomFichier(listeFichiers **fichiers, char *nom_fichier) {

  if (fichiers && (*fichiers)->nom == NULL) {
    (*fichiers)->nom = nom_fichier;
  }
  else {
    listeFichiers *nouveau = malloc(sizeof(struct listeFichiers));
    nouveau->nom = nom_fichier;
    nouveau->suiv = *fichiers;
    *fichiers = nouveau;
  }
}

listeFichiers* freeListeFichiers(listeFichiers *fichier) {
  
  if (fichier) {
    listeFichiers *suiv;
    suiv = fichier->suiv;
    free(fichier->nom);
    free(fichier);
    return suiv;
  }
  return NULL;
}

void printListeFichiers(listeFichiers *fichiers) {
  
  if (fichiers) {
    printf("%s", fichiers->nom);
    listeFichiers *fichier = fichiers->suiv;
    while(fichier) {
      printf(", %s", fichier->nom);
      fichier = fichier->suiv;
    }
    printf("\n");
  }
}

int estVide(file *file) {

    if(file && file->tete) {
        return 0;
    }
    return 1;
}

element* initElement(int id) {

    element *nouveau = malloc(sizeof(element));
    nouveau->id = id;
    nouveau->suiv = NULL;
    return nouveau;
}

file* initFile(int id) {

    file *f = malloc(sizeof(file));
    f->tete = initElement(id);
    f->queue = f->tete;
    return f;
}
 
void ajouterDansFile(file *file, int id) {
    
    if (estVide(file)) {
        file->tete = initElement(id);
        file->queue = file->tete;
    }
    else {
        file->queue->suiv = initElement(id);
        file->queue = file->queue->suiv;
    }
}
 
element defiler(file *file) {

    element copie;
    element *temp = file->tete;
    copie = *temp;
    file->tete = file->tete->suiv;
    free(temp);
    return copie;
}

void printFile(file *file) {

    if (file) {
        element *suiv = file->tete;
        while (suiv) {
            printf("->%d", suiv->id);
            suiv = suiv->suiv;
        }
        printf("\n");
    }
}

listeCycles* initListeCycles() {

    listeCycles *cycles = malloc(sizeof(listeCycles));
    cycles->cycles = NULL;
    cycles->nb_cycles = 0;
    return cycles;
}

void ajouterCycleDansListe(listeCycles *cycles, cycle c) {

  if(cycles->cycles == NULL) {
		cycles->cycles = malloc((cycles->nb_cycles + 1) * sizeof(cycle));
	}
	else {
		cycle *temp = realloc(cycles->cycles, (cycles->nb_cycles + 1) * sizeof(cycle));
    
    if (temp == NULL) {
      printf("Erreur de réallocation de la liste de cycles.\n");
      exit(EXIT_FAILURE);
    }
    cycles->cycles = temp;
	}
	cycles->cycles[cycles->nb_cycles] = c;
  (cycles->nb_cycles)++;
}

void freeListeCycles(listeCycles *cycles) {

  if (cycles) {
    if (cycles->cycles) {
      free(cycles->cycles);
    }
    free(cycles);
  }
}

void freeTousListeCycles(listeCycles *cycles) {

  if (cycles) {
    if (cycles->cycles) {
      int i;
      for (i = 0; i < cycles->nb_cycles; i++) {
        if (cycles->cycles[i].sommets) {
          free(cycles->cycles[i].sommets);
        }
      }
      free(cycles->cycles);
    }
    free(cycles);
  }
}

void printListeCycles(listeCycles *cycles) {

  int i;
  printf("Liste cycles :\n");
  for (i = 0; i < cycles->nb_cycles; i++) {
    printf("c%d : ", i);
    printTab(cycles->cycles[i].sommets, cycles->cycles[i].taille);
  }
  printf("\n");
}

indexCycles* initIndexCycles(int taille) {

  indexCycles *index_cycles = malloc(taille * sizeof(indexCycles));
  index_cycles->taille = taille;
  int i, j;

  for (i = 0; i < taille; i++) {
    index_cycles[i].cycles = malloc(SIZE_INDEX * sizeof(int));

    for (j = 0; j < SIZE_INDEX; j++) {
      index_cycles[i].cycles[j] = -1;
    }
  }

  return index_cycles;
}

void resetIndexCycles(indexCycles *index_cycles, int taille) {

  int i, j;
  for (i = 0; i < taille; i++) {
    j = 0;
    while (j < SIZE_INDEX && index_cycles[i].cycles[j] != -1) {
      index_cycles[i].cycles[j] = -1;
      j++;
    }
  }
}

void ajouterCycleDansIndex(indexCycles *index_cycles, int id_sommet, int id_cycle) {
    
  if (id_sommet > index_cycles->taille) {
    printf("Erreur : la taille de buffer allouée pour indexer les cycles");
    printf(" est inférieure à la taille du graphe moléculaire. %d  %d\n", id_sommet,index_cycles->taille);
    exit(EXIT_FAILURE);
  }

  int i = 0;
  while (i < SIZE_INDEX && index_cycles[id_sommet].cycles[i] != -1) {
    i++;
  }
  if (i == SIZE_INDEX) {
    // TODO faire un realloc
    printf("Erreur interne : taille de buffer SIZE_INDEX est trop petite.\n");
    exit(EXIT_FAILURE);
  }
  else {
    index_cycles[id_sommet].cycles[i] = id_cycle;
  }
}

void freeIndexCycles(indexCycles *index_cycles) {

  int i ;
  for (i = 0; i < index_cycles->taille; i++) {
    free(index_cycles[i].cycles);
  }
  free(index_cycles);
}

void printIndexCycles(indexCycles *index_cycles, int taille) {

  int i, j;
  printf("Index cycles (taille %d) :\n", taille);
  if (taille <= 0 || taille > index_cycles->taille) {
    taille = index_cycles->taille;
  }
  for (i = 0; i < taille; i++) {
    printf("%d :", i);
    j = 0;
    while (j < SIZE_INDEX && index_cycles[i].cycles[j] /*!= -1*/) {
      printf(" %d", index_cycles[i].cycles[j]);
      j++;
    }
    printf("\n");
  }
   printf("\n");
}

grapheCycles initGrapheCycles(listeCycles *cycles, int chebi_id, int nb_atomes, char *types) {

  grapheCycles g;
  g.chebi_id = chebi_id;
  g.nb_atomes = nb_atomes;
  g.types = types;
  g.nb_sommets = cycles->nb_cycles;
  g.nb_aretes = 0;
  g.types_aretes = NULL;
  g.sommets = malloc(cycles->nb_cycles * sizeof(sommet));
  int i;

  for (i = 0; i < cycles->nb_cycles; i++) {
    g.sommets[i].id = i;
    g.sommets[i].taille = cycles->cycles[i].taille;
  }

  return g;
}

arete initArete(int id1, int id2, int type, int poids) {

  arete a;
  a.id1 = id1;
  a.id2 = id2;
  a.type = type;
  a.poids = poids;

  return a;
}

// Crée une arête entre les sommets id1 et id2 et l'ajoute à la liste aretes.
// Initialise la liste aretes si elle est vide.  
void ajouterAreteDansListe(listeAretes **aretes, int *nb_aretes, int id1, int id2, int type, int poids) {

  if (*aretes == NULL) {
    *aretes = malloc(sizeof(listeAretes));
    (*aretes)->a = initArete(id1, id2, type, poids);
    (*aretes)->suiv = NULL;
  }
  else {
    listeAretes *nouvelle = malloc(sizeof(listeAretes));
    nouvelle->a = initArete(id1, id2, type, poids);
    nouvelle->suiv = *aretes;
    *aretes = nouvelle;
  }
  (*nb_aretes)++;
}

void freeListeAretes(listeAretes *aretes) {

  listeAretes *temp;
  while (aretes) {
    temp = aretes;
    aretes = aretes->suiv;
    free(temp);
  }
}

// Insère les arêtes de la liste dans le tableau aretes de g,
// et libère la mémoire allouée à la liste.  
void ajouterAreteDansGraphe(grapheCycles *g, listeAretes *aretes, int nb_aretes) {

  int i, j;
  g->types_aretes = malloc(g->nb_sommets * sizeof(typeArete*));

  for (i = 0; i < g->nb_sommets; i++) {
    g->types_aretes[i] = malloc(g->nb_sommets * sizeof(typeArete));

    for (j = 0; j < g->nb_sommets; j++) {	
			g->types_aretes[i][j].type = AUCUNE_LIAISON;
			g->types_aretes[i][j].poids = AUCUNE_LIAISON;
		}
  }

  g->nb_aretes = nb_aretes;
  
  listeAretes *temp;
  while (aretes) {
    temp = aretes;
    aretes = aretes->suiv;

    g->types_aretes[temp->a.id1][temp->a.id2].poids = temp->a.poids;
    g->types_aretes[temp->a.id2][temp->a.id1].poids = temp->a.poids;
    g->types_aretes[temp->a.id1][temp->a.id2].type = temp->a.type;
    g->types_aretes[temp->a.id2][temp->a.id1].type = temp->a.type;
    
    free(temp);
  }
}

void freeGrapheCycles(grapheCycles g) {

  int i;
  free(g.sommets);
  for (i = 0; i < g.nb_sommets; i++) {
    free(g.types_aretes[i]);
  }
  free(g.types_aretes);
  free(g.types);
}

void printGrapheCycles(grapheCycles g) {

  int i, j;

  printf("Graphe de cycles (t,p) :\n");
  if (g.types_aretes) {
    for (i = 0; i < g.nb_sommets; i++) {
      for (j = 0; j < g.nb_sommets; j++) {
        printf("%d,%d ", g.types_aretes[i][j].type, g.types_aretes[i][j].poids);
      }
      printf("\n");
    }
  }
  printf("Taille des cycles\n");
    for (i = 0; i < g.nb_sommets; i++) {
      printf("%d ", g.sommets[i].taille);
    }
    printf("\n");  
}
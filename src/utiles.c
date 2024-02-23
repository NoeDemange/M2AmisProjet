#include "utiles.h"

void* allouer(size_t taille, char *message) {
    void * ptr;
    ptr = malloc(taille);
    if(ptr == NULL) {
        printf("[Problème d'allocation] %s\n.", message);
        exit(EXIT_FAILURE);
    }
    return ptr;
}

void* callouer(size_t nb_el, size_t taille, char *message) {
    
    void * ptr;
    ptr = calloc (nb_el, taille);
    if(ptr == NULL) {
        printf("[Problème d'allocation] %s\n.", message);
        exit(EXIT_FAILURE);
    }
    return ptr;
}

void* reallouer(void *ptr, size_t taille, char *message) {

    void * nv_ptr;
    nv_ptr = realloc(ptr, taille);
    if(nv_ptr == NULL) {
        printf("[Problème de reallocation] %s\n.", message);
        exit(EXIT_FAILURE);
    }
    return nv_ptr;
}

FILE* ouvrirFichier(const char *nom_fichier, const char *mode) {

    FILE *f = fopen(nom_fichier, mode);
    if (f == NULL) {
        printf("Erreur lors de l'ouverture du fichier %s.\n", nom_fichier);
        exit(EXIT_FAILURE);
    }
    return f;
}

double chrono() {
  
	struct timeval tv;
	static double date_deb = 0.0;
	double date_courante;
	gettimeofday(&tv, NULL);
	date_courante = tv.tv_sec + ((double) tv.tv_usec) * 1e-6;
	if (date_deb == 0.0) {
        date_deb = date_courante;
    }
	return date_courante - date_deb;
}

void tempsExecution(double sec, char *info) {

  int h, m;
  double s;
  h = (sec/3600); 
  m = (sec -(3600*h))/60;
  s = (sec -(3600*h)-(m*60));
	
  printf("%s - %dh:%dm:%fs\n",info, h, m, s);	
}

void scannerFichier(int valeur, char *chebi_id) {
  
  if (valeur != 1) {
    printf("Erreur de lecture du fichier pour la molécule %s.\n", chebi_id);
    exit(EXIT_FAILURE);
  }
}

char* allouerChaine(char *chaine) {

  size_t taille_allouee = strlen(chaine) ;
  char *nouveau = (char *)malloc(taille_allouee + 1);
  strcpy(nouveau, chaine);

  return nouveau;
}

void creerDossier(const char *chemin) {
    
    #ifdef _WIN32
        mkdir(chemin);
    #else
        mkdir(chemin, 0777);
    #endif
}

void printTab(int *tab, int n) {
    
    int i;
    for (i = 0; i < n; i++)
        printf("%d ", tab[i]);
    printf("\n");
}

void printMatrice(int **matrice, int n, int m) {

  int i, j;
  for (i = 0; i < n; i++) {
    for (j = 0; j < m; j++) {
      printf(" %d", matrice[i][j]);
    }
    printf("\n");
  }
  printf("\n");
}
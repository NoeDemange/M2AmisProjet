#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h> 
#include <getopt.h>

#include "fichiers.h"
#include "utiles.h"
#include "Horton.h"

void usage() {
	printf(USAGE_FMT);
	exit(1);
}

int main(int argc, char** argv) {

    double debut, fin, sec;

    struct stat  file_stat; 

    if (stat("data", &file_stat) < 0) {
        printf("[ERREUR] Pas de fichiers trouvés. Saisissez : make data\n");
        exit(0);
    }

    debut = chrono();

    int option;

    options opt = {NULL, NULL, 0, 0};

    while ((option = getopt(argc, argv, OPTSTR)) != EOF) {
        switch (option) {
        case 'a':
            opt.chebi_id1 = optarg;
            break;
        case 'b':
            opt.chebi_id2 = optarg;
            break;
        case 'n':
            opt.nb_fichiers = atoi(optarg);
            break;
        case 'g':
            opt.graphe = 1;
            break;
        case 'h':
        default:
            usage();
            break;
        }
    }

    if (opt.chebi_id1 == NULL) {
        if (opt.nb_fichiers <= 0) {
            printf("Comparaison de toutes les molécules entre elles.\n");
            procedure("data", 0, NULL);
        }
        else {
            printf("Comparaison de %d molécules entre elles.\n", opt.nb_fichiers);
            procedure("data", opt.nb_fichiers, NULL);
        }
    }
    else {
        if (opt.chebi_id2) {
            printf("Comparaison de CHEBI:%s et CHEBI:%s.\n", opt.chebi_id1, opt.chebi_id2);
            if (opt.graphe) {
                printf("[OPTION] génération des graphes de cycles. Après l'exécution, saisissez : make png\n");
            }
            comparaison("data", opt.chebi_id1, opt.chebi_id2, opt.graphe);
        }
        else {
            if (opt.nb_fichiers <= 0) {
                printf("Comparaison de toutes les molécules à CHEBI:%s.\n", opt.chebi_id1);
            }
            else {
                printf("Comparaison de %d molécules à CHEBI:%s.\n", opt.nb_fichiers, opt.chebi_id1);
            }
            procedure("data", opt.nb_fichiers, opt.chebi_id1);
        }
    }
    fin = chrono();

    sec = fin - debut;
    tempsExecution(sec, "Temps d'exécution");
    
    return EXIT_SUCCESS;
}
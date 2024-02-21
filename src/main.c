#include <stdlib.h>
#include <stdio.h>

#include "fichiers.h"
#include "utiles.h"
#include "Horton.h"

int main(int argc, char** argv) {
    
    // Test_Horton();

    double debut, fin, sec;
    debut = chrono();
    procedure("data", 0);
    fin = chrono();
    sec = fin - debut;
    tempsExecution(sec, "Temps d'exécution");
    
    return EXIT_SUCCESS;
}
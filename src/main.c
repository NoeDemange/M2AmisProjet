#include <stdlib.h>
#include <stdio.h>

#include "Dijkstra.h"
#include "Horton.h"
#include "McKay.h"

int main(int argc, char** argv) {
    testMcKay();
    Test_Horton();
    
    return EXIT_SUCCESS;
}
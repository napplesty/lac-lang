#include "global.h"
#include "processeur.h"
#include "executeur.h"
#include "file.h"

int main (int argc, char *argv[]) {
    if(argc != 2) {
        printf("input format: \"./runtime [file name]\"\n");
        return 0;
    }
    processeurInitialize();
    lacTableInitialize();
    vmTableInitialize();
    stackInitialize();
    registerInitialize();
    registerAllProcesseur();

    fileToState(argv[1]);
    run();
    return 0;
}
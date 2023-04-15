#include "global.h"
#include "processeur.h"
#include "compilation.h"
#include "file.h"

int main (int argc, char *argv[]) {
    if(argc != 3) {
        printf("input format: \"./runtime [file name] [output file name]\"\n");
        return 0;
    }
    processeurInitialize();
    lacTableInitialize();
    vmTableInitialize();
    stackInitialize();
    registerInitialize();
    registerAllProcesseur();

    char *s = getfileall(argv[1]);
    compileString(s);
    currentStateToFile(argv[2]);

    return 0;
}
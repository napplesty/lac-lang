#include "common.h"
#include "compilation.h"
#include "global.h"
#include "executeur.h"

int main() {
    processeurInitialize();
    lacTableInitialize();
    vmTableInitialize();
    stackInitialize();
    registerInitialize();
    registerAllProcesseur();
    
    char *s = getfileall("./examples/factorielle.lac");
    compileString(s);
    run();
    return 0;
}
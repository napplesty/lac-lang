#include "common.h"
#include "compilation.h"
#include "global.h"

int main() {
    processeurInitialize();
    lacTableInitialize();
    vmTableInitialize();
    stackInitialize();
    registerInitialize();
    registerAllProcesseur();
    
    char *s = getfileall("./examples/factorielle.lac");
    compileString(s);

    funcInfoPtr query = makeQueryEntry();
    query->funcName = "go";
    int nfa = find(query);
    free(query);
    funcInfoPtr result = parseWithNfa(nfa);
    if(result == NULL) {
        printf("This function not exists\n");
    } else {
        printFuncInfo(result);
    }
    freeFound(result);

    printProgram();
    printCompiledLac("go");

    return 0;
}
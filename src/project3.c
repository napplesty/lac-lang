#include "find.h"
#include "global.h"
#include "processeur.h"

int main() {
    processeurInitialize();
    vmTableInitialize();
    lacTableInitialize();
    registerInitialize();
    stackInitialize();

    registerAllProcesseur();
    
    // look up with function name
    funcInfoPtr query = makeQueryEntry();
    query->funcName = ";";
    int nfa = find(query);
    funcInfoPtr result = parseWithNfa(nfa);
    if(result == NULL) {
        printf("This function not exists\n");
    } else {
        printFuncInfo(result);
    }

    // look up with nfa
    query = makeQueryEntry();
    query->nfa = 1;
    nfa = find(query);
    result = parseWithNfa(nfa);
    if(result == NULL) {
        printf("This function not exists\n");
    } else {
        printFuncInfo(result);
    }

    // look up with cfa
    query = makeQueryEntry();
    query->cfa = 2;
    nfa = find(query);
    result = parseWithNfa(nfa);
    if(result == NULL) {
        printf("This function not exists\n");
    } else {
        printFuncInfo(result);
    }

    // look up with numero processeur
    query = makeQueryEntry();
    query->numeroProcesseur = 0;
    nfa = find(query);
    result = parseWithNfa(nfa);
    if(result == NULL) {
        printf("\n------------------\n");
        printf("This function not exists\n");
    } else {
        printFuncInfo(result);
    }

    // look up with joint key
    query = makeQueryEntry();
    query->numeroProcesseur = 0;
    query->cfa = 0;
    nfa = find(query);
    result = parseWithNfa(nfa);
    if(result == NULL) {
        printf("------------------\n");
        printf("This function not exists\n");
    } else {
        printFuncInfo(result);
    }

    // look up failure if joint key does not exist
    query = makeQueryEntry();
    query->numeroProcesseur = 0;
    query->cfa = 5;
    nfa = find(query);
    result = parseWithNfa(nfa);
    if(result == NULL) {
        printf("------------------\n");
        printf("This function not exists\n");
    } else {
        printFuncInfo(result);
    }

    return 0;
}

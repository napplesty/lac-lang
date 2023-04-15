#include "find.h"

int findWithFuncName(const char *funcName) {
    if(checkAllReady() == -1) {exception(__FILE__, __LINE__, "Not initialized");}
    int length = strlen(funcName);
    int idx = 1;
    while(lacTable[idx] != 0 && idx < LACSIZE) {
        if(lacTable[idx] == length) {
            for(int i = 0; i < length; i++) {
                if((int)funcName[i] != lacTable[idx+1+i]) {
                    break;
                } else if(i == length - 1) {
                    return idx;
                }
            }
        }
        idx += lacTable[idx] + 3;
    }
    return -1;
}

// just for processeur
int findWithNumeroProcesseur(int numeroProcesseur) {
    if(checkAllReady() == -1) {exception(__FILE__, __LINE__, "Not initialized");}
    if(processeur[numeroProcesseur] == NULL) {
        return -1;
    } else {
        int idx =  1, length = 0, cfa = -1;
        while(lacTable[idx] != 0 && idx < LACSIZE) {
            length = lacTable[idx];
            cfa = lacTable[idx + length + 1];
            if(vmTable[cfa+1] == numeroProcesseur) {return idx;}
            idx += lacTable[idx] + 3;
        }
    }
    return -1;
}

int findWithCfa(int cfa) {
    if(checkAllReady() == -1) {exception(__FILE__, __LINE__, "Not initialized");}
    if((vmTable[cfa] == 0 && cfa < LACBEGIN) || (vmTable[cfa] == -1 && cfa >= LACBEGIN)) {
        return -1;
    } else {
        int idx =  1, length = 0;
        while(lacTable[idx] != 0 && idx < LACSIZE) {
            length = lacTable[idx];
            if(cfa == lacTable[idx + length + 1]) {return idx;}
            idx += lacTable[idx] + 3;
        }
    }
    return -1;
}

int findWithNfa(int nfa) {
    if(checkAllReady() == -1) {exception(__FILE__, __LINE__, "Not initialized");}
    int idx = 1;
    while(lacTable[idx] != 0 && idx <= nfa) {
        if(idx == nfa) {return nfa;}
        idx += lacTable[idx] + 3;
    }
    return -1;
}

int find(funcInfoPtr func) {
    if(checkAllReady() == -1) {exception(__FILE__, __LINE__, "Not initialized");}
    const char *funcName = func->funcName;
    int numeroProcesseur = func->numeroProcesseur;
    int cfa = func->cfa;
    int nfa = func->nfa;

    int nfas[4], check[4];
    for(int i = 0; i < 4; i++) {
        nfas[i] = -1;
        check[i] = -1;
    }

    if(funcName != NULL) {
        check[0] = 0;
        nfas[0] = findWithFuncName(funcName);
    }
    if(numeroProcesseur != -1) {
        check[1] = 0;
        nfas[1] = findWithNumeroProcesseur(numeroProcesseur);
    }
    if(cfa != -1) {
        check[2] = 0;
        nfas[2] = findWithCfa(cfa);
    }
    if (nfa != -1) {
        check[3] = 0;
        nfas[3] = findWithNfa(nfa);
    }

    for(int i = 0; i < 4; i++) {
        if(nfas[i] != -1) {
            if(nfa == -1) {
                nfa = nfas[i];
            } else if(nfa != nfas[i]) {
                return -1;
            }
        } else if(check[i] == 0) {
            return -1;
        }
    }
    return nfa;
}

funcInfoPtr parseWithNfa(int nfa) {
    if(checkAllReady() == -1) {exception(__FILE__, __LINE__, "Not initialized");};
    if(findWithNfa(nfa) == -1) {return NULL;}
    funcInfoPtr ret = malloc(sizeof(struct funcInfo));
    
    int length = lacTable[nfa];
    char *funcName = malloc(length+1);
    funcName[length] = '\0';
    for(int i = 0; i < length; i++) {
        funcName[i] = (char)lacTable[nfa+1+i];
    }
    int cfa = lacTable[nfa+length+1];
    int numeroProcesseur = vmTable[cfa+1];
    if(cfa >= LACBEGIN) {
        numeroProcesseur = PROCESSEURSIZE;
    }

    ret->numeroProcesseur = numeroProcesseur;
    ret->cfa = cfa;
    ret->nfa = nfa;
    ret->funcName = funcName;

    return ret;
}

void freeFound(funcInfoPtr found) {
    free((char *)found->funcName);
    free(found);
    return;
}

funcInfoPtr makeQueryEntry() {
    funcInfoPtr ret = malloc(sizeof(struct funcInfo));
    ret->funcName = NULL;
    ret->cfa = -1;
    ret->nfa = -1;
    ret->numeroProcesseur = -1;
    return ret;
}

void printFuncInfo(funcInfoPtr func) {
    printf("\n------------------------------\nfunction information\n");
    printf("function name: %s\n", func->funcName);
    printf("nfa: %d\n", func->nfa);
    printf("cfa: %d\n", func->cfa);
    if(func->numeroProcesseur >= PROCESSEURSIZE) {
        printf("numero processeur: This is a lac function\n");
    } else {
        printf("numero processeur: %d\n", func->numeroProcesseur);
    }
}

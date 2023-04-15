#include "executeur.h"

void run() {
    if(checkAllReady() == -1) {exception(__FILE__, __LINE__, "Not Ready");}

    int numeroProcesseur=0, cfa=0;
    while (vmTable[registerFile.pc] != -1) {        
        cfa = vmTable[registerFile.pc];
        if(cfa < LACBEGIN && vmTable[cfa] == -1) {
            numeroProcesseur = vmTable[cfa+1];
            processeur[numeroProcesseur]();
        } else if(cfa >= LACBEGIN) {
            debutCompilation();
        } else {
            exception(__FILE__, __LINE__, "program not right");
        }
        registerFile.pc ++;
        printOutput();
        resetOutput();
        //printDataPile();
        //printReturnPile();
        //usleep(100*1000);
    }
    return;
}


void printOutput() {
    for(int i = OUTPUTBUFFERBEGIN; i <= registerFile.tib; i++) {
        if(vmTable[i] == 0) {
            continue;
        }
        printf("%c", (char)vmTable[i]);
    }
}

void resetOutput() {
    for(int i = OUTPUTBUFFERBEGIN; i <= registerFile.tib; i++) {
        vmTable[i] = 0;
    }
    registerFile.tib = OUTPUTBUFFERBEGIN;
}

#include "global.h"

// lac and VM part
base processeur[PROCESSEURSIZE];
int lacTable[LACSIZE];
int vmTable[VMSIZE];
int initializationCheck = 0;

void processeurInitialize() {
    memset(processeur, '\0', sizeof(processeur));
    initializationCheck = initializationCheck | PROCESSEURREADY;
}

int processeurFuncRegister(base func) {
    if(checkAllReady() == -1) {exception(__FILE__, __LINE__, "Not initialized");}
    for(int i = 0; i < PROCESSEURSIZE; i++) {
        if(processeur[i] == NULL) {
            processeur[i] = func;
            return i;
        }
    }
    exception(__FILE__, __LINE__, "Number of processeur out of volume");
    return -1;
}

// VM and LAC
void lacTableInitialize() {
    memset(lacTable, '\0', sizeof(lacTable));
    initializationCheck = initializationCheck | LACREADY;
}

void vmTableInitialize() {
    memset(lacTable, '\0', sizeof(lacTable));
    for(int i = LACBEGIN - 1; i < OUTPUTBUFFERBEGIN; i++) {
        vmTable[i] = -1;
    }
    initializationCheck = initializationCheck | VMREADY;
}

int lacRegister(const char *funcName, int cfa) {
    if(checkAllReady() == -1) {exception(__FILE__, __LINE__, "Not initialized");}
    int length = strlen(funcName);
    int idx = 1;
    while(lacTable[idx] != 0) {
        idx += lacTable[idx] + 3;
        if(idx + length + 3 >= LACSIZE) {exception(__FILE__, __LINE__, "lac Table space for processeur is not enough");}
    }
    lacTable[idx] = length;
    for(int i = 1; i < length + 1; i ++) {
        lacTable[idx+i] = funcName[i-1];
    }
    lacTable[idx+length+1] = cfa;
    lacTable[idx+length+2] = idx; // nfa
    return idx;
}

// just for processeur
int vmRegister(int numeroProcesseur) {
    if(checkAllReady() == -1) {exception(__FILE__, __LINE__, "Not initialized");}
    for(int i = 0; i < min(PROCESSEURSIZE * 2, LACBEGIN - 1); i += 2) {
        if (vmTable[i] == 0) {
            vmTable[i] = -1;
            vmTable[i+1] = numeroProcesseur;
            return i;
        }
    }
    exception(__FILE__, __LINE__, "VM space for processeur is not enough");
    return -1;
}

// just for processeur
funcInfoPtr registerFunc(base func, const char *funcName) {
    if(checkAllReady() == -1) {exception(__FILE__, __LINE__, "Not initialized");}
    funcInfoPtr ret = malloc(sizeof(struct funcInfo));
    int numeroProcesseur = processeurFuncRegister(func);
    int cfa = vmRegister(numeroProcesseur);
    int nfa = lacRegister(funcName, cfa);
    ret->funcName = funcName;
    ret->numeroProcesseur = numeroProcesseur;
    ret->cfa = cfa;
    ret->nfa = nfa;
    return ret;
}

int checkAllReady() {
    if (initializationCheck == (PROCESSEURREADY | LACREADY | VMREADY | STACKREADY | REGISTERREADY)) {
        return 0;
    }
    return -1;
}

// for debug

void printLac() {
    for(int i = 0; i < 50; i++) {
        printf("%d ", lacTable[i]);
    }
    printf("\n");
}


void printVm() {
    for(int i = 0; i < 50; i++) {
        printf("%d ", vmTable[i]);
    }
    printf("\n");
}











// stack part
pile dataStack;
int dataStackMemory[4096];
pile returnStack;
int returnStackMemory[1024];

void stackInitialize() {
    registerFile.sp = -1;
    registerFile.ra = -1;
    initializationCheck = initializationCheck | STACKREADY;
    for(int i = 0; i < STACKSIZE; i++) {
        dataStackMemory[i] = 0;
    }
    for(int i = 0; i < RETURNSTACKSIZE; i++) {
        returnStackMemory[i] = 0;
    }
    dataStack.pop = pilePop;
    dataStack.push = pilePush;
    returnStack.pop = returnPilePop;
    returnStack.push = returnPilePush;
}

int pilePop(void) {
    if(registerFile.sp == -1) {exception(__FILE__, __LINE__, "Pop an empty data pile");}
    return dataStackMemory[registerFile.sp--];
}

void pilePush(int elem) {
    if(registerFile.sp == STACKSIZE - 1) {exception(__FILE__, __LINE__, "Push a full data pile");}
    dataStackMemory[++registerFile.sp] = elem;
}

int returnPilePop(void) {
    if(registerFile.ra == -1) {exception(__FILE__, __LINE__, "Pop an empty return pile");}
    return returnStackMemory[registerFile.ra--];
}

void returnPilePush(int elem) {
    if(registerFile.ra == RETURNSTACKSIZE - 1) {exception(__FILE__, __LINE__, "Push a full return pile");}
    returnStackMemory[++registerFile.ra] = elem;
}

void printDataPile() {
    if(checkAllReady() == -1) {exception(__FILE__, __LINE__, "not ready");}
    printf("------------\n");
    printf("Data stack state: \n");
    for(int i = 0; i <= registerFile.sp; i++) {
        printf("%d ", dataStackMemory[i]);
    }
    printf("\n");
}

void printReturnPile() {
    if(checkAllReady() == -1) {exception(__FILE__, __LINE__, "not ready");}
    printf("------------\n");
    printf("Return stack state: \n");
    for(int i = 0; i <= registerFile.ra; i++) {
        printf("%d ", returnStackMemory[i]);
    }
    printf("\n");
}



// register part
vmRuntimeRegister registerFile;

void registerInitialize() {
    registerFile.sp = -1;
    registerFile.ra = -1;
    registerFile.pc = PROGRAMENTRY; // entry of execution
    registerFile.tib = OUTPUTBUFFERBEGIN;
    initializationCheck = initializationCheck | REGISTERREADY;
}

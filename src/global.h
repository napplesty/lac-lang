#include "common.h"
#include "analex.h"

#ifndef GLOBAL_H
#define GLOBAL_H

#define VMREADY         (1<<0)
#define PROCESSEURREADY (1<<1)
#define LACREADY        (1<<2)
#define STACKREADY      (1<<3)
#define REGISTERREADY   (1<<4)

#define PROCESSEURSIZE (30)
#define LACSIZE (4096)
#define VMSIZE (4096*3)
#define OUTPUTBUFFERBEGIN (4096*2)
#define PROGRAMENTRY 4096
#define LACBEGIN 1024
#define PROCESSEURBEGIN (0)

// LAC and VM part
typedef void (*base)(void); // Type de la fonction de base
extern base processeur[PROCESSEURSIZE];
extern int lacTable[LACSIZE];
extern int vmTable[VMSIZE];
extern int initializationCheck;

// just free it, never mind the memory leak
typedef struct funcInfo* funcInfoPtr;
struct funcInfo {
    const char *funcName; // must be a literal
    int numeroProcesseur;
    int cfa;
    int nfa;
};

// les fonctions du processeur
void processeurInitialize();
int processeurFuncRegister(base func);

// VM and LAC
void lacTableInitialize();
void vmTableInitialize();
int lacRegister(const char *funcName, int cfa);
int vmRegister(int numeroProcesseur);

// regester a processeur
funcInfoPtr registerFunc(base func, const char *funcName);

// check
int checkAllReady();

// debug
void printLac();
void printVm();






// Pile part

#define STACKSIZE (sizeof(dataStackMemory)/sizeof(int))
#define RETURNSTACKSIZE (sizeof(returnStackMemory)/sizeof(int))

typedef struct {
    int (*pop)(void);
    void (*push)(int);
} pile;
typedef pile* pilrPtr;

extern pile dataStack;
extern int dataStackMemory[4096];
extern pile returnStack;
extern int returnStackMemory[1024];

void stackInitialize();
int pilePop(void);
void pilePush(int elem);
int returnPilePop(void);
void returnPilePush(int elem);

// for debug
void printDataPile();
void printReturnPile();


// registers for execution (just like cpu :-) )
typedef struct {
    int ra;    // return address
    int sp;    // saved register
    int pc;    // point to current code
    int tib;   // pointor of buffer for output
} vmRuntimeRegister;

extern vmRuntimeRegister registerFile;
void registerInitialize();

#endif

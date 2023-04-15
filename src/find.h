#include "common.h"
#include "global.h"

#ifndef FIND_H
#define FIND_H

// find

int findWithFuncName(const char *funcName); // in lac
int findWithNumeroProcesseur(int numeroProcesseur); // in vm
int findWithCfa(int cfa); // in lac
int findWithNfa(int nfa); // in lac
int find(funcInfoPtr func);

funcInfoPtr parseWithNfa(int nfa);
void freeFound(funcInfoPtr found);
funcInfoPtr makeQueryEntry();
void printFuncInfo(funcInfoPtr func);

#endif

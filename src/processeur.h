#include "common.h"
#include "global.h"
#include "anasynt.h"

#ifndef PROCESSEUR_H
#define PROCESSEUR_H

void plus(void);                // numero processeur: 0
void swap(void);                // numero processeur: 1
void codeDup(void);             // numero processeur: 2
void mylit(void);               // numero processeur: 3
void moins(void);               // numero processeur: 4
void myfin(void);               // numero processeur: 5
void debutCompilation(void);    // numero processeur: 6
void egal(void);                // numero processeur: 7
void condition(void);           // numero processeur: 8
void drop(void);                // numero processeur: 9
void conditionBranchElse(void); // numero processeur: 10
void recurse(void);             // numero processeur: 11
void fois(void);                // numero processeur: 12
void conditionBranchThen(void); // numero processeur: 13
void finCompilation(void);      // numero processeur: 14
void stringBegin(void);         // numero processeur: 15
void count(void);               // numero processeur: 16
void codeType(void);            // numero processeur: 17
void point(void);               // numero processeur: 18
void cr(void);                  // numero processeur: 19
void calculate(void);           // numero processeur: 20
void str(void);                 // numero processeur: 21
void diviser(void);             // numero processeur: 22

void registerAllProcesseur();


#endif
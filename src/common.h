#include <stdio.h>
#include <regex.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifndef COMMON_H
#define COMMON_H
// stack operations
typedef struct linkedListNode* linkedListNodePtr;
typedef struct linkedStack* linkedStackPtr;
struct linkedListNode {
    void *val;
    linkedListNodePtr next;
    linkedListNodePtr prev;
};
struct linkedStack{
    linkedListNodePtr begin;
    linkedListNodePtr end;
    int len;
};

linkedStackPtr createStack();
void deleteStack(linkedStackPtr stack);
void stackPush(linkedStackPtr stack, void *val);
void *stackPop(linkedStackPtr stack);

// regex operations
struct regexResult {
    int begin;
    int end;
    int status;
};
struct regexResult *regexFind(const char* s, const char* pat);

// string operations
void skipToken(char *s);
char *makeCopy(const char *s);
int min(int a, int b);
int max(int a, int b);

// arbre operations
typedef struct noeud* arbre;
struct noeud{
    char* etiquette;
    arbre fils_gauche;
    arbre fils_droit;
    arbre parent;
};
void print_arbre(arbre a);
void print_arbre_decale(arbre a, int d);

// entry for exception
void exception(const char * fileName, int line, const char *errMsg);

// file operations
char *getfileall(const char *fname);
#endif

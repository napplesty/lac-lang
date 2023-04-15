#include "common.h"

#ifndef ANASYNT_H
#define ANASYNT_H

typedef int (*match_stack_op_t)(char*, linkedStackPtr);

arbre AST(const char *s);
arbre ASTConstructorSkeleton(char *s, match_stack_op_t* opFuncs, int opFuncLength);

// recognize elements in the string and construct an AST node
int entier(char *s, linkedStackPtr stack);
int opLp(char *s, linkedStackPtr stack);
int opHp(char *s, linkedStackPtr stack);
int leftParenthese(char *s, linkedStackPtr stack);
int rightParenthese(char *s, linkedStackPtr stack);

// identify the element in the ettiquette
int isEntier(const char *s);
int isOp(const char *s);
int isOpLp(const char *s);
int isOpHp(const char *s);
int isLeftParenthese(const char *s);
int isRightParenthese(const char *s);
int isEntierNoeud(arbre root);
int verifyArbre(arbre root);

arbre createNoeud(const char *s);
void deleteNoeud(arbre root);
void deleteArbre(arbre root);
//void squeezeArbre(arbre root);

// for execution
int calculateTree(arbre root);

#endif
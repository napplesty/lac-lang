#include "common.h"
#include "global.h"
#include "processeur.h"
#include "find.h"
#include "analex.h"

#ifndef COMPILATION_H
#define COMPILATION_H

void compileLambda(const char *s);
void compileLac(const char *funcName, const char *funcContent);
funcInfoPtr compileDefinedFunction(struct token *tok);
funcInfoPtr compileBase(const char *funcName, base func);      // just an alias for registerFunc() in global.h
int calculateCodeLength(struct token *tok);
void compileIfElseStatement(struct token *tok, int *bitToFill); // handle complicated if else statement (support nested if else statement)

void compile(struct token *tok);
void compileString(const char *s);

// help function
void printProgram();
void printCompiledLac(const char *funcName);

#endif
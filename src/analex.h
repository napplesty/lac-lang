#include "common.h"

#ifndef ANALEX_H
#define ANALEX_H

struct token {
    char *tok;
    char type;
    struct token *next;
};

typedef struct token *(*tokenizer_t)(char *);

struct token *tokenize(const char *s); 
struct token *tokenizeOneStep(char *s, tokenizer_t *tokenizers, int tokenizersLength);

// tokenizer
struct token *identificateur(char *s);
struct token *entier_relatif(char *s);
struct token *chaines_de_caracteres(char *s);
struct token *commentaires_de_ligne(char *s);
struct token *commentaires_multilignes(char *s);

// helper functions
void printTokens(struct token *li);
int tokenLength(struct token *li);
struct token *createToken(const char *s, char Type);
void deleteToken(struct token *tok);
void deleteTokens(struct token *tok);
#endif

#include "analex.h"

// 复制字符串并保存到token里
struct token *createToken(const char *s, char type) {
    int length = strlen(s);
    struct token *ret = (struct token *)calloc(1,sizeof(struct token));
    char *s_tmp = (char *)calloc(length+1,1);
    memmove(s_tmp,s,length);
    ret->tok=s_tmp;
    ret->type=type;
    return ret;
}

// 通过createToken创建的token都可以用这个函数进行正确的释放
void deleteToken(struct token* tok) {
    free(tok->tok);
    free(tok);
    return;
}

void deleteTokens(struct token *tok) {
    struct token *prev;
    while(tok != NULL) {
        prev = tok;
        tok = tok->next;
        deleteToken(prev);
    }
    return;
}

int tokenLength(struct token *li) {
    struct token *iter = li;
    int ret = 0;
    for(; iter != NULL; iter=iter->next) {ret++;}
    return ret;
}

void printTokens(struct token *li) {
    if(li == NULL) {return;}
    struct token *iter = li;
    char *currentTok;
    int length;
    for(int i = 0; iter != NULL; iter=iter->next) {
        if(i != 0) {printf(" -> ");}
        i++;
        currentTok = iter->tok;
        length = strlen(currentTok);
        printf("%c(\"",iter->type);
        for(int j = 0; j < length; j++) {
            if(currentTok[j] == '\n')
                printf("\\n"); // 为了不打印换行符
            else
                printf("%c", currentTok[j]); 
        }
        printf("\")");
    }
    printf("\n");
}

struct token *tokenize(const char *s) {
    struct token *lastToken = NULL, *newToken, *firstToken = NULL;
    int length = strlen(s);
    char *tmp = (char *)malloc(length + 1);
    memmove(tmp, s, length);
    tmp[length] = '\0';

    tokenizer_t tokenizers[] = {entier_relatif,
                                chaines_de_caracteres,
                                commentaires_de_ligne,
                                commentaires_multilignes,
                                identificateur};

    while(*tmp != '\0') {
        newToken = tokenizeOneStep(tmp, tokenizers, 5);
        if(newToken == NULL) {continue;}
        if(newToken->type == 'C') {deleteToken(newToken);continue;}
        if(lastToken == NULL) {
            lastToken = newToken;
            firstToken = lastToken;
        } else {
            lastToken->next = newToken;
            lastToken = lastToken->next;
        }
    }
    free(tmp);
    return firstToken;
}

struct token *tokenizeOneStep(char *s, tokenizer_t *tokenizers, int tokenizersLength) {
    struct token *ans;
    skipToken(s);
    for(int i = 0; i < tokenizersLength && *s != '\0'; i++) {
        ans = tokenizers[i](s);
        if(ans != NULL) {
            return ans;
        }
    }
    return NULL;
}

struct token *identificateur(char *s) {
    int length = strlen(s);
    struct token *ret;
    struct regexResult *matchResult = regexFind(s, "[^ \n]+( |\n|$)");
    if(matchResult->begin != 0||matchResult->status == REG_NOMATCH) {return NULL;};

    char tmp[matchResult->end+1];
    memmove(tmp, s, matchResult->end);
    if(length == matchResult->end) 
        tmp[matchResult->end] = '\0';
    else
        tmp[matchResult->end-1] = '\0';
    
    memmove(s,s+matchResult->end, length - matchResult->end);
    s[length - matchResult->end] = '\0';
    ret = createToken(tmp,'M');
    free(matchResult);
    return ret;
}

struct token *entier_relatif(char *s) {
    int length = strlen(s);
    struct token *ret;
    struct regexResult *matchResult = regexFind(s, "[+|-]?[0-9]+( |\n|$)");
    if(matchResult->begin != 0||matchResult->status == REG_NOMATCH) {return NULL;};

    char tmp[matchResult->end+1];
    memmove(tmp, s, matchResult->end);
    if(length == matchResult->end) 
        tmp[matchResult->end] = '\0';
    else
        tmp[matchResult->end-1] = '\0';
    
    memmove(s,s+matchResult->end, length - matchResult->end);
    s[length - matchResult->end] = '\0';
    ret = createToken(tmp,'N');
    free(matchResult);
    return ret;
}

struct token *chaines_de_caracteres(char *s) {
    int length = strlen(s);
    struct token *ret;
    struct regexResult *matchResult = regexFind(s, "[\"][ ]([^\"]|\n)*[\"]");
    if(matchResult->begin != 0||matchResult->status == REG_NOMATCH) {return NULL;};

    char tmp[matchResult->end+1];
    memmove(tmp, s+2, matchResult->end-3);
    tmp[matchResult->end-3] = '\0';
    
    memmove(s,s+matchResult->end, length - matchResult->end);
    s[length - matchResult->end] = '\0';
    ret = createToken(tmp,'S');
    free(matchResult);
    return ret;
}

struct token *commentaires_de_ligne(char *s) {
    int length = strlen(s);
    struct token *ret;
    struct regexResult *matchResult = regexFind(s, "[\\][ ][^\n]*[\n]");
    if(matchResult->begin != 0||matchResult->status == REG_NOMATCH) {return NULL;};

    char tmp[matchResult->end+1];
    memmove(tmp, s, matchResult->end);
    tmp[matchResult->end] = '\0';
    
    memmove(s,s+matchResult->end, length - matchResult->end);
    s[length - matchResult->end] = '\0';
    ret = createToken(tmp,'C');
    free(matchResult);
    return ret;
}

struct token *commentaires_multilignes(char *s) {
    int length = strlen(s);
    struct token *ret;
    struct regexResult *matchResult = regexFind(s, "[(][ ][^)]*[)]");
    if(matchResult->begin != 0||matchResult->status == REG_NOMATCH) {return NULL;};
    char tmp[matchResult->end+1];
    memmove(tmp, s, matchResult->end);
    tmp[matchResult->end] = '\0';
    
    memmove(s,s+matchResult->end, length - matchResult->end);
    s[length - matchResult->end] = '\0';
    ret = createToken(tmp,'C');
    free(matchResult);
    return ret;
}

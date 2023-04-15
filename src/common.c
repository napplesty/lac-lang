#include "common.h"

// stack operations
linkedStackPtr createStack() {
    linkedStackPtr stack = (linkedStackPtr)calloc(1,sizeof(struct linkedStack));
    stack->begin = (linkedListNodePtr)calloc(1,sizeof(struct linkedListNode));
    stack->end = (linkedListNodePtr)calloc(1,sizeof(struct linkedListNode));
    stack->begin->next = stack->end;
    stack->end->prev = stack->begin;
    stack->len = 0;
    return stack;
}

void deleteStack(linkedStackPtr stack) {
    linkedListNodePtr tmp = stack->begin, tmp_prev;
    for(int i = 0; i < stack->len + 1; i++) {
        tmp = tmp->next;
        tmp_prev = tmp->prev;
        if(tmp_prev->val != NULL)
            free(tmp_prev->val);
        free(tmp_prev);
    }
    if(tmp->val != NULL)
        free(tmp->val);
    free(tmp);
    return;
}

void stackPush(linkedStackPtr stack, void *val) {
    linkedListNodePtr elem = (linkedListNodePtr)calloc(1,sizeof(struct linkedListNode));
    elem->val = val;
    elem->next = stack->end;
    elem->prev = stack->end->prev;
    elem->next->prev = elem;
    elem->prev->next = elem;
    stack->len ++;
    return;
}

void *stackPop(linkedStackPtr stack) {
    if(stack->len == 0) {
        exception(__FILE__, __LINE__, "No element in stack");
    }
    linkedListNodePtr lastElem = stack->end->prev;
    lastElem->prev->next = stack->end;
    stack->end->prev = lastElem->prev;
    void *ret = lastElem->val;
    stack->len --;
    free(lastElem);
    return ret;
}


//regex operations
struct regexResult *regexFind(const char* s, const char* pat) {
    regex_t pattern;
    regmatch_t result;
    char buf[1025];
    struct regexResult *ret = (struct regexResult *)calloc(1,sizeof(struct regexResult));
    if(regcomp(&pattern, pat, REG_EXTENDED)) {
        perror("Regex invalid\n");
        printf("%s\n", pat);
        regfree(&pattern);
        exit(0);
    }
    memmove(buf, s, min(1024, strlen(s)));
    buf[min(1024, strlen(s))] = '\0';
    int status = regexec(&pattern, buf, 1, &result, 0);
    ret->status = status;
    ret->begin = result.rm_so;
    ret->end = result.rm_eo;
    regfree(&pattern);
    return ret;
}

// string operations
void skipToken(char *s) {
    int n = strlen(s), begin = 0, end = n-1;;
    if(s[0] != ' ' && s[0] != '\n' && s[n-1] == ' ' && s[n-1] == '\n') {
        return;
    }
    char *tmp = (char*)malloc(n+1);
    memmove(tmp, s, n);
    tmp[n] = '\0';
    while(begin < n && (tmp[begin] == ' ' || tmp[begin] == '\n')) {begin++;}
    while(end > begin && (tmp[end] == ' ' || tmp[end] == '\n')) {end--;}
    memmove(s,tmp+begin,end+1-begin);
    s[end+1-begin] = '\0';
    free(tmp);
    return;
}

char *makeCopy(const char *s) {
    int length = strlen(s);
    char *ret = (char *)malloc(length+1);
    ret[length] = '\0';
    memmove(ret, s, length);
    return ret;
}

int min(int a, int b) {return a<b?a:b;}
int max(int a, int b) {return a>b?a:b;}

// arbre operations
void print_arbre(arbre a) {
    print_arbre_decale(a, 0);
}

void print_arbre_decale(arbre a, int d) {
    int i;
    int l;
    if (a == NULL)
        return;
    l = strlen(a->etiquette);
    print_arbre_decale(a->fils_gauche, d + l + 1);
    for (i = 0; i < d; i++)
        printf(" ");
    printf("%s\n", a->etiquette);
    print_arbre_decale(a->fils_droit, d + l + 1);
}

void exception(const char * fileName, int line, const char *errMsg) {
    printf("Something go wrong here: %s: line %d\n", fileName, line);
    printf("Error: %s\n", errMsg);
    exit(0);
}



// file operations
char *getfileall(const char *fname) {
    FILE *fp;
    char *str;
    char txt[1000];
    int filesize;
    fp = fopen(fname,"r");
    fseek(fp,0,SEEK_END);
    filesize = ftell(fp);
    str=(char *)calloc(filesize+1,1);
    str[0]=0;
    rewind(fp);
    while((fgets(txt,1000,fp))!=NULL){
        strcat(str,txt);
    }
    fclose(fp);
    return str;
}

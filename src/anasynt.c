#include "anasynt.h"

arbre createNoeud(const char *s) {
    arbre ret = (arbre)calloc(1, sizeof(struct noeud));
    ret->etiquette = makeCopy(s);
    return ret;
}

void deleteNoeud(arbre root) {
    if(root == NULL)
        return;
    if(root->etiquette != NULL)
        free(root->etiquette);
    free(root);
}

void deleteArbre(arbre root) {
    if(root != NULL) {
        deleteArbre(root->fils_droit);
        deleteArbre(root->fils_gauche);
        deleteNoeud(root);
    }
}

arbre AST(const char *s) {
    int length = strlen(s);
    char *sCopy = (char *)malloc(length+1);
    sCopy[length] = '\0'; memmove(sCopy, s, length);
    match_stack_op_t opStackFuncs[] = {entier,opLp,opHp,leftParenthese,rightParenthese};

    arbre root = ASTConstructorSkeleton(sCopy, opStackFuncs, 5);
    free(sCopy);
    return root;
}

arbre ASTConstructorSkeleton(char *s, match_stack_op_t* opFuncs, int opFuncLength) {
    int signalForFuncAccept = 0;
    linkedStackPtr stack = createStack();
    arbre root = createNoeud("waiting");
    stackPush(stack, root);

    while(*s != '\0') {
        signalForFuncAccept = -1;
        skipToken(s);
        for(int i = 0; i < opFuncLength; i++) {
            signalForFuncAccept = signalForFuncAccept & opFuncs[i](s, stack);
        }
        if(signalForFuncAccept == -1) {
            exception(__FILE__, __LINE__, "Unkonwn character");
        }
    }
    if(stack->len != 1) {
        exception(__FILE__, __LINE__, "Parenthese not correspond");
    }
    //squeezeArbre(root);

    arbre currentNoeud = (arbre)stackPop(stack);
    arbre currentParent = currentNoeud->parent;
    while(currentParent != NULL) {
        currentNoeud = currentParent;
        currentParent = currentNoeud->parent;
    }
    deleteStack(stack);
    if (verifyArbre(currentNoeud) != 0) {
        print_arbre(currentNoeud);
        exception(__FILE__, __LINE__, "Bad expression");
    }
    return currentNoeud;
}

int isEntier(const char *s) {
    struct regexResult *matchResult = regexFind(s, "[0-9]+");
    if (matchResult->begin == 0 && matchResult->end == strlen(s) && matchResult->status != REG_NOMATCH) {
        free(matchResult);
        return 0;
    }
    free(matchResult);
    return -1;
}

int isOp(const char *s) {
    if (strlen(s) != 1)
        return -1;
    switch(*s) {
        case '+': return 0;
        case '-': return 0;
        case 'x': return 0;
        case '/': return 0;
        default: return -1;
    }
}

int isOpLp(const char *s) {
    if (strlen(s) != 1) 
        return -1;
    switch(*s) {
        case '+': return 0;
        case '-': return 0;
        default: return -1;
    }
}

int isOpHp(const char *s) {
    if (strlen(s) != 1) 
        return -1;
    switch(*s) {
        case 'x': return 0;
        case '/': return 0;
        default: return -1;
    }
}

int isLeftParenthese(const char *s) {
    if (strlen(s) != 1) 
        return -1;
    if (*s == '(')
        return 0;
    return -1;
}

int isRightParenthese(const char *s) {
    if (strlen(s) != 1) 
        return -1;
    if (*s == ')')
        return 0;
    return -1;
}

int isEntierNoeud(arbre root) {
    while (root != NULL && isOpLp(root->etiquette) == 0 && root->fils_gauche == NULL) {
        root = root->fils_droit;
    } 
    if(root == NULL || 
    root->fils_gauche != NULL ||
    root->fils_droit != NULL || 
    isEntier(root->etiquette) !=  0) {
        return -1;
    }
    return 0;
}

int verifyArbre(arbre root) {
    if(root == NULL) {
        return -1;
    } else if (isOpHp(root->etiquette) == 0) {
        if(root->fils_droit == NULL || root->fils_gauche == NULL) {
            return -1;
        }
        return verifyArbre(root->fils_gauche) | verifyArbre(root->fils_droit);
    } else if(isOpLp(root->etiquette) == 0) {
        if(root->fils_gauche == NULL) {
            return isEntierNoeud(root) & verifyArbre(root->fils_droit);
        } else if (root->fils_droit != NULL) {
            return verifyArbre(root->fils_gauche) | verifyArbre(root->fils_droit);
        } else {
            return -1;
        }
    } else if (isEntier(root->etiquette) == 0) {
        return 0;
    }
    return -1;
}

int entier(char *s, linkedStackPtr stack) {
    int length = strlen(s);
    struct regexResult *matchResult = regexFind(s, "[0-9]+");
    if (matchResult->begin != 0||matchResult->status == REG_NOMATCH) {free(matchResult); return -1;}
    char tmp[matchResult->end+1];
    memmove(tmp, s, matchResult->end);
    tmp[matchResult->end] = '\0';
    memmove(s,s+matchResult->end, length - matchResult->end);
    s[length - matchResult->end] = '\0';

    arbre currentNoeud = (arbre)stackPop(stack);
    if(strcmp(currentNoeud->etiquette, "waiting") == 0) {
        free(currentNoeud->etiquette);
        currentNoeud->etiquette = makeCopy(tmp);
        stackPush(stack, currentNoeud);
    } else if(isOp(currentNoeud->etiquette) == 0) {
        arbre fils_droit = createNoeud(tmp);
        currentNoeud->fils_droit = fils_droit;
        fils_droit->parent = currentNoeud;
        stackPush(stack, fils_droit);
    } else {
        exception(__FILE__, __LINE__, "Bad expression");
    }

    free(matchResult);
    return 0;
}

int opLp(char *s, linkedStackPtr stack) {
    int length = strlen(s);
    struct regexResult *matchResult = regexFind(s, "[+|-]");
    if (matchResult->begin != 0||matchResult->status == REG_NOMATCH) {free(matchResult); return -1;}
    char tmp[matchResult->end+1];
    memmove(tmp, s, matchResult->end);
    tmp[matchResult->end] = '\0';
    memmove(s,s+matchResult->end, length - matchResult->end);
    s[length - matchResult->end] = '\0';

    arbre currentNoeud = (arbre)stackPop(stack);
    if(strcmp(currentNoeud->etiquette, "waiting") == 0) {
        free(currentNoeud->etiquette);
        currentNoeud->etiquette = makeCopy(tmp);
        stackPush(stack, currentNoeud);
        free(matchResult);
        return 0;
    } else if (isEntier(currentNoeud->etiquette) == 0 || (verifyArbre(currentNoeud) == 0)) {
        arbre currentParent = currentNoeud->parent;
        while(currentParent != NULL) {
            currentNoeud = currentParent;
            currentParent = currentNoeud->parent;
        }
        currentParent = createNoeud(tmp);
        currentParent->fils_gauche = currentNoeud;
        currentNoeud->parent = currentParent;
        stackPush(stack, currentParent);
    } else if (isOpHp(currentNoeud->etiquette) == 0) {
        currentNoeud->fils_droit = createNoeud(tmp);
        currentNoeud->fils_droit->parent = currentNoeud;
        stackPush(stack, currentNoeud->fils_droit);
    } else {
        exception(__FILE__, __LINE__, "Bad expression");
    }
    free(matchResult);
    return 0;
}

int opHp(char *s, linkedStackPtr stack) {
    int length = strlen(s);
    struct regexResult *matchResult = regexFind(s, "[x|/]");
    if (matchResult->begin != 0||matchResult->status == REG_NOMATCH) {free(matchResult); return -1;}
    char tmp[matchResult->end+1];
    memmove(tmp, s, matchResult->end);
    tmp[matchResult->end] = '\0';
    memmove(s,s+matchResult->end, length - matchResult->end);
    s[length - matchResult->end] = '\0';

    arbre currentNoeud = (arbre)stackPop(stack);
    if(strcmp(currentNoeud->etiquette, "waiting") == 0) {
        free(currentNoeud->etiquette);
        currentNoeud->etiquette = makeCopy(tmp);
        stackPush(stack, currentNoeud);
        free(matchResult);
        return 0;
    } else if (verifyArbre(currentNoeud) == 0) {
        arbre currentParent = currentNoeud->parent;
        arbre currentFils = createNoeud(tmp);
        if (currentParent != NULL && isEntierNoeud(currentParent) == 0 ) {
            currentNoeud = currentParent;
            currentParent = currentNoeud->parent;
        }
        while (currentParent != NULL && isOpHp(currentParent->etiquette) == 0) {
            currentNoeud = currentParent;
            currentParent = currentNoeud->parent;
        }
        if(currentParent == NULL) {
            currentFils->fils_gauche = currentNoeud;
            currentNoeud->parent = currentFils;
        } else {
            currentFils->parent = currentParent;
            currentFils->fils_gauche = currentNoeud;
            currentNoeud->parent = currentFils;
            currentParent->fils_droit = currentFils;
        }
        stackPush(stack, currentFils);
    } else {
        exception(__FILE__, __LINE__, "Bad expression");
    }
    free(matchResult);
    return 0;
}

int leftParenthese(char *s, linkedStackPtr stack) {
    int length = strlen(s);
    struct regexResult *matchResult = regexFind(s, "[(]");
    if (matchResult->begin != 0||matchResult->status == REG_NOMATCH) {free(matchResult); return -1;}
    char tmp[matchResult->end+1];
    memmove(tmp, s, matchResult->end);
    tmp[matchResult->end] = '\0';
    memmove(s,s+matchResult->end, length - matchResult->end);
    s[length - matchResult->end] = '\0';

    arbre currentNoeud = stackPop(stack);
    if(strcmp(currentNoeud->etiquette, "waiting") == 0) {
        arbre newRoot = createNoeud("waiting");
        stackPush(stack, currentNoeud);
        stackPush(stack, newRoot);
    } else if(currentNoeud->fils_droit == NULL && isEntierNoeud(currentNoeud) != 0) {
        currentNoeud->fils_droit = createNoeud("waiting");
        currentNoeud->fils_droit->parent = currentNoeud;
        arbre newRoot = createNoeud("waiting");
        stackPush(stack, currentNoeud->fils_droit);
        stackPush(stack, newRoot);
    } else {
        exception(__FILE__, __LINE__, "Bad expression");
    }
    return 0;
}

int rightParenthese(char *s, linkedStackPtr stack) {
    int length = strlen(s);
    struct regexResult *matchResult = regexFind(s, "[)]");
    if (matchResult->begin != 0||matchResult->status == REG_NOMATCH) {free(matchResult); return -1;}
    char tmp[matchResult->end+1];
    memmove(tmp, s, matchResult->end);
    tmp[matchResult->end] = '\0';
    memmove(s,s+matchResult->end, length - matchResult->end);
    s[length - matchResult->end] = '\0';

    arbre currentNoeud = (arbre)stackPop(stack);
    arbre prevNoeud = (arbre)stackPop(stack);
    arbre currentParent = currentNoeud->parent;
    while(currentParent != NULL) {
        currentNoeud = currentParent;
        currentParent = currentNoeud->parent;
    }
    if(prevNoeud->parent != NULL) {
        prevNoeud->parent->fils_droit = currentNoeud;
        currentNoeud->parent = prevNoeud->parent;
    }
    deleteNoeud(prevNoeud);
    stackPush(stack, currentNoeud);
    return 0;
}


// for execution
int calculateTree(arbre root) {
    switch (root->etiquette[0]) {
    case '+':
        if(root->fils_gauche == NULL) {
            return calculateTree(root->fils_droit);
        } else {
            return calculateTree(root->fils_gauche) + calculateTree(root->fils_droit);
        }
    case '-':
        if(root->fils_gauche == NULL) {
            return 0 - calculateTree(root->fils_droit);
        } else {
            return calculateTree(root->fils_gauche) - calculateTree(root->fils_droit);
        }
    case 'x':
        return calculateTree(root->fils_gauche) * calculateTree(root->fils_droit);
    case '/':
        return calculateTree(root->fils_gauche) / calculateTree(root->fils_droit);
    default:
        return atoi(root->etiquette);
    }
}

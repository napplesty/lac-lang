#include "compilation.h"

funcInfoPtr compileBase(const char *funcName, base func) {
    funcInfoPtr ret = registerFunc(func, funcName);
    return ret;
}

void compileLac(const char *funcName, const char *funcContent) {
    if(checkAllReady() == -1) {exception(__FILE__, __LINE__, "Not ready");}
    if(findWithFuncName(funcName) != -1) {return;}
    funcInfoPtr ret = makeQueryEntry();
    funcInfoPtr query = NULL;
    struct token *tok = tokenize(funcContent);
    struct token *prev = tok;

    // find the space for lac functions
    int cfa = LACBEGIN;
    int tmpNfa = 0, tmpCfa = 0;
    while(cfa < PROGRAMENTRY && vmTable[cfa] != -1) {
        cfa ++;
    }
    if(cfa == PROGRAMENTRY) {exception(__FILE__, __LINE__, "No enough space in VM for lac functions");}
    ret->cfa = cfa;

    while(tok != NULL) {
        prev = tok;
        tok = tok->next;
        if(prev->type == 'M') {
            tmpNfa = findWithFuncName(prev->tok);
            query = parseWithNfa(tmpNfa);
            if(query == NULL) {
                exception(__FILE__, __LINE__, "Function not defined");
            }
            tmpCfa = query->cfa;
            vmTable[cfa++] = tmpCfa;
        } else if(prev->type == 'N') {
            tmpNfa = findWithFuncName("(lit)");
            query = parseWithNfa(tmpNfa);
            if(query == NULL) {
                exception(__FILE__, __LINE__, "Function not defined");
            }
            tmpCfa = query->cfa;
            vmTable[cfa++] = tmpCfa;
            vmTable[cfa++] = atoi(prev->tok);
        } else {
            exception(__FILE__, __LINE__, "bug in analex");
        }
        freeFound(query);
        deleteToken(prev);
    }

    tmpNfa = findWithFuncName("(fin)");
    query = parseWithNfa(tmpNfa);
    if(query == NULL) {
        exception(__FILE__, __LINE__, "Function not defined");
    }
    tmpCfa = query->cfa;
    vmTable[cfa++] = tmpCfa;
    freeFound(query);

    ret->nfa = lacRegister(funcName, ret->cfa);  // register to lac
    free(ret);
    return;
}

void compileLambda(const char *s) {
    if(checkAllReady() == -1) {exception(__FILE__, __LINE__, "Not ready");}
    int length = strlen(s);
    char *ret = NULL;
    struct regexResult *foundCalcul = regexFind(s, "[+|-]?[0-9]+[-|+|\\*|/|=]");
    if(foundCalcul->begin == 0 && foundCalcul->end == length && foundCalcul->status != REG_NOMATCH) {
        ret = malloc(length + 2);
    }
    if (ret != NULL) {
        for(int i = 0; i < length - 1; i++) {
            ret[i] = s[i];
        }
        ret[length-1] = ' ';
        ret[length] = s[length-1];
        ret[length+1] = '\0';
        compileLac(s, ret);
        free(ret);
    }
    free(foundCalcul);
    return;
}

funcInfoPtr compileDefinedFunction(struct token *tok) {
    if(checkAllReady() == -1) {exception(__FILE__, __LINE__, "Not ready");}
    if(strcmp(tok->tok, ":") != 0) {exception(__FILE__, __LINE__, "this function is not used rightly");}
    char *funcName = tok->next->tok;
    int cfa = LACBEGIN, tmpCfa = 0, tmpNfa = 0, length = 0;
    funcInfoPtr ret = makeQueryEntry(), tmp = NULL;

    // encode vm
    while(cfa < PROGRAMENTRY && vmTable[cfa] != -1) {
        cfa++;
    }
    ret->cfa = cfa;

    // reserve enough volume for compile
    int codeLength = calculateCodeLength(tok); 
    for(int i = cfa; i < cfa + codeLength; i++){
        vmTable[i] = -2;
    }

    tok = tok->next->next;
    while(strcmp(tok->tok, ";") != 0){
        if(tok->type == 'M') {
            if(strcmp(tok->tok, "recurse") == 0) {
                vmTable[cfa++] = ret->cfa;
                tmp = makeQueryEntry();
            } else if(strcmp(tok->tok, "if") == 0 || strcmp(tok->tok, "else") == 0) {
                compileIfElseStatement(tok, &cfa);
                tmp = makeQueryEntry();
            } else {
                compileLambda(tok->tok);
                tmpNfa = findWithFuncName(tok->tok);
                tmp = parseWithNfa(tmpNfa);
                if(tmp == NULL) {
                    exception(__FILE__, __LINE__, "Function not defined");
                }
                vmTable[cfa++] = tmp->cfa;
            }
        } else if (tok->type == 'N') {
            tmpNfa = findWithFuncName("(lit)");
            tmp = parseWithNfa(tmpNfa);
            if(tmp == NULL) {
                exception(__FILE__, __LINE__, "Function not defined");
            }
            tmpCfa = tmp->cfa;
            vmTable[cfa++] = tmpCfa;

            vmTable[cfa++] = atoi(tok->tok);
        } else if (tok->type == 'S') {
            tmpNfa = findWithFuncName("str");
            tmp = parseWithNfa(tmpNfa);
            if(tmp == NULL) {
                exception(__FILE__, __LINE__, "Function not defined");
            }
            tmpCfa = tmp->cfa;
            vmTable[cfa++] = tmpCfa;

            length = strlen(tok->tok);
            vmTable[cfa++] = length;
            for(int i = 0; i < length; i++) {
                vmTable[cfa++] = (int)tok->tok[i];
            }
        } else {
            exception(__FILE__, __LINE__, "bug in analex");
        }
        freeFound(tmp);
        tok = tok->next;
    }

    tmpNfa = findWithFuncName(";");
    tmp =parseWithNfa(tmpNfa);
    if(tmp == NULL) {
        exception(__FILE__, __LINE__, "Function not defined");
    }
    vmTable[cfa++] = tmp->cfa;
    freeFound(tmp);

    // func Name register
    lacRegister(funcName, ret->cfa);
    return ret;
}

int calculateCodeLength(struct token *tok) {
    int codeLength = -1; 
    struct token *copie = tok;
    while (strcmp(copie->tok, ";") != 0) {
        if(copie->type == 'M') {
            codeLength ++;
            if(strcmp(copie->tok, "if") == 0) {
                codeLength += 2;
            }
        } else if(copie->type == 'N') {
            codeLength += 2;
        } else if(copie->type == 'S') {
            codeLength += strlen(copie->tok)+1;
        }
        copie = copie->next;
    }
    return codeLength;
}

void compileIfElseStatement(struct token *tok, int *bitToFill) {
    int currentPc = *bitToFill;

    // encode current
    int tmpNfa = findWithFuncName(tok->tok);
    funcInfoPtr tmp = parseWithNfa(tmpNfa);
    vmTable[currentPc ++] = tmp->cfa;
    freeFound(tmp);

    // "if" -> "else" and "else" -> "then"
    char buf[5];
    if(strcmp(tok->tok, "if") == 0) {
        buf[0] = 'e';
        buf[1] = 'l';
        buf[2] = 's';
        buf[3] = 'e';
    } else {
        buf[0] = 't';
        buf[1] = 'h';
        buf[2] = 'e';
        buf[3] = 'n';
    }
    buf[4] = '\0';
 
    // find next postion
    int stackCnt = 1;
    int position = 0;
    tok = tok->next;
    while(stackCnt > 0 && tok != NULL) {
        if(strcmp(tok->tok, "if") == 0) {
            stackCnt ++;
        } else if(strcmp(tok->tok, buf) == 0) {
            stackCnt --;
        }

        // calculate jump distence
        if(tok->type == 'M') {
            position ++;
            if(strcmp(tok->tok, "if") == 0) {
                position += 2;
            }
        } else if(tok->type == 'N') {
            position += 2;
        } else if(tok->type == 'S') {
            position += strlen(tok->tok)+1;
        }
        tok = tok->next;
    }

    vmTable[currentPc] = currentPc + position;
    currentPc ++;
    *bitToFill = currentPc;
}





void compile(struct token *tok) {
    if(checkAllReady() == -1) {exception(__FILE__, __LINE__, "Not ready");}
    int pc = registerFile.pc, tmpNfa = 0, length = 0;
    funcInfoPtr tmp = NULL;
    while (tok != NULL) {
        if(tok->type == 'M') {
            if(strcmp(tok->tok, ":") == 0) {
                compileDefinedFunction(tok);
                while(strcmp(tok->tok, ";") != 0) {
                    tok = tok->next;
                }
            } else if(strcmp(tok->tok, "if") == 0 || strcmp(tok->tok, "else") == 0) {
                compileIfElseStatement(tok, &pc);
            } else {
                compileLambda(tok->tok);
                tmpNfa = findWithFuncName(tok->tok);
                tmp = parseWithNfa(tmpNfa);

                vmTable[pc++] = tmp->cfa; 
                freeFound(tmp);
            }
        } else if(tok->type == 'N') {
            tmpNfa = findWithFuncName("(lit)");
            tmp = parseWithNfa(tmpNfa);

            vmTable[pc++] = tmp->cfa;
            freeFound(tmp);

            vmTable[pc++] = atoi(tok->tok);
        } else if(tok->type == 'S') {
            tmpNfa = findWithFuncName("str");
            tmp = parseWithNfa(tmpNfa);

            vmTable[pc++] = tmp->cfa;
            freeFound(tmp);

            length = strlen(tok->tok);
            vmTable[pc++] = length;
            for(int i = 0; i < length; i++) {
                vmTable[pc++] = tok->tok[i];
            }
        }
        tok = tok->next;
    }
}

void compileString(const char *s) {
    struct token *tok = tokenize(s);
    struct token *copie = tok;
    compile(tok);
    deleteTokens(copie);
    return;
}

void printProgram() {
    if(checkAllReady() == -1) {exception(__FILE__, __LINE__, "Not Ready");}
    printf("---------------\nCurrent Program:\n");
    for(int i = registerFile.pc; i < OUTPUTBUFFERBEGIN; i++) {
        if(vmTable[i] == -1) {
            break;
        }
        printf("%d ", vmTable[i]);
    }
    printf("\n");
}

void printCompiledLac(const char *funcName) {
    if(checkAllReady() == -1) {exception(__FILE__, __LINE__, "Not Ready");}
    int tmpNfa = 0, tmpCfa = 0, finCfa = 0, functionEndCfa = 0;
    funcInfoPtr tmp = NULL;
    tmpNfa = findWithFuncName(funcName);
    tmp = parseWithNfa(tmpNfa);
    if(tmp == NULL) {
        exception(__FILE__, __LINE__, "This function not exist");
    }
    printf("---------------------\n");
    printf("Function in LAC: \n");
    for(int i = tmpNfa; i <= tmpNfa + lacTable[tmp->nfa] + 2; i++) {
        printf("%d ", lacTable[i]);
    }
    printf("\n");
    printf("Function in VM: \n");
    tmpCfa = tmp->cfa;
    freeFound(tmp);
    tmpNfa = findWithFuncName("(fin)");
    tmp = parseWithNfa(tmpNfa);
    finCfa = tmp->cfa;
    freeFound(tmp);
    tmpNfa = findWithFuncName(";");
    tmp = parseWithNfa(tmpNfa);
    functionEndCfa = tmp->cfa;
    freeFound(tmp);

    for(int i = tmpCfa; i == tmpCfa || (vmTable[i-1] != finCfa && vmTable[i-1] != functionEndCfa); i++) {
        printf("%d ", vmTable[i]);
    }
    printf("\n");
}

#include "processeur.h"


// don't know the meaning of this function
// not shown in homework
void swap(void) {
    return;
}

void plus(void) {
    if(checkAllReady() == -1) {exception(__FILE__, __LINE__, "Stack not ready");}
    dataStack.push(dataStack.pop() + dataStack.pop());
}

void moins(void) {                        // in gcc: 
    int b = dataStack.pop();              // if write: 0 - dataStack.pop() + dataStack.pop()
    int a = dataStack.pop();              //                                 |||||||||||||| : this will be run first
    dataStack.push(a-b);                  // in clang:     ||||||||||||| : this will be run first
}
void fois(void) {
    dataStack.push(dataStack.pop() * dataStack.pop());
}

void diviser(void) {
    int b = dataStack.pop(), a = dataStack.pop();
    dataStack.push(a/b);
}

// duplique la valeur sur le sommet de la pile de données
void codeDup(void) {
    dataStack.push(dataStackMemory[registerFile.sp]);
}

// signaler à l’exécuteur que la valeur suivante est une donnée à mettre sur la pile de données
void mylit(void) {
    dataStack.push(vmTable[++registerFile.pc]);
}

// signaler à l’exécuteur que la fonction en cours se termine
void myfin(void) {
    registerFile.pc = returnStack.pop();
}

// signale un début de compilation, le terme après est le nom de la fonction dans LAC
void debutCompilation(void) {
    returnStack.push(registerFile.pc);
    registerFile.pc = vmTable[registerFile.pc] - 1; // auto increasion of pc will get it right
}

// teste si les deux valeurs sur la pile de données sont égales, dépose -1 sur la pile de données si elles sont égales et 0 sinon
void egal(void) {
    if(dataStack.pop() == dataStack.pop()) {
        dataStack.push(-1);             // -1 is true
    } else {
        dataStack.push(0);              // 0 is false
    }
}

// teste si a valeur sur le sommet de la pile de données est -1 ou 0 et effectue le code qui suit jusqu’au prochain else ou then
void condition(void) {
    int compareResult = dataStack.pop();
    if(compareResult == -1) { 
        registerFile.pc ++;   // because of auto increasion
    } else {                  // goto else
        registerFile.pc = vmTable[registerFile.pc+1]+1;
    }
}

// efface la valeur sur le sommet de la pile de données
void drop(void) {
    dataStack.pop();
}

// effectue le code qui suit jusqu’au prochain then
void conditionBranchElse(void) {
    registerFile.pc = vmTable[registerFile.pc+1];
}

// do nothine (even not have to be compiled)
// fin de la structure ... if ... else ... then
void conditionBranchThen(void) {
    return;
}

// se compile avec le Cfa de la fonction en cours (ici, la fonction fact)
void recurse(void) {
    // handle in compile (not possible to implement with current runtime components)
    // this is a void implemention
    return;
}

// don't know the difference with (fin) maybe is the same.
void finCompilation(void) {
    registerFile.pc = returnStack.pop();
}

// qui prévient l’exécuteur qu’arrive une chaîne de caractères
void stringBegin(void) {
    int length = vmTable[++registerFile.pc];
    dataStack.push(registerFile.pc); // push string address to  stack
    registerFile.pc += length;
}

void count(void) {
    int addr = dataStack.pop();
    int length = vmTable[addr];
    dataStack.push(addr + 1);
    dataStack.push(length);
}

void codeType(void) {
    int length = dataStack.pop();
    int addr = dataStack.pop();
    for(int i = 0; i < length; i++) {
        vmTable[registerFile.tib++] =  vmTable[addr++];
    }
}

// imprime la valeur sur le sommet de la pile de données
// have to make the value a chain of character
void point(void) {
    int val = dataStack.pop();
    char buf[20];
    snprintf(buf, 19, "%d", val);
    int length = strlen(buf);
    for(int i = 0; i < length; i++) {
        vmTable[registerFile.tib++] = buf[i];
    }
}

void cr(void) {
    vmTable[registerFile.tib++] = (int)'\n';
}

void calculate(void) {
    int addr = dataStack.pop();
    int length = vmTable[addr];
    char buf[length+1];
    buf[length] = '\0';
    for(int i = 0; i < length; i++) {
        buf[i] = (char)vmTable[addr+i+1];
    }
    arbre root = AST(buf);
    dataStack.push(calculateTree(root));
    deleteArbre(root);
}

void str(void) {
    stringBegin();
}

void registerAllProcesseur() {
    if(checkAllReady() == -1) {exception(__FILE__, __LINE__, "Not initialized");}
    free(registerFunc(plus,                 "+"));             // numero processeur: 0
    free(registerFunc(swap,                 "swap"));          // numero processeur: 1
    free(registerFunc(codeDup,              "dup"));           // numero processeur: 2 
    free(registerFunc(mylit,                "(lit)"));         // numero processeur: 3   
    free(registerFunc(moins,                "-"));             // numero processeur: 4
    free(registerFunc(myfin,                "(fin)"));         // numero processeur: 5    
    free(registerFunc(debutCompilation,     ":"));             // numero processeur: 6
    free(registerFunc(egal,                 "="));             // numero processeur: 7
    free(registerFunc(condition,            "if"));            // numero processeur: 8 
    free(registerFunc(drop,                 "drop"));          // numero processeur: 9   
    free(registerFunc(conditionBranchElse,  "else"));          // numero processeur: 10   
    free(registerFunc(recurse,              "recurse"));       // numero processeur: 11      
    free(registerFunc(fois,                 "*"));             // numero processeur: 12
    free(registerFunc(conditionBranchThen,  "then"));          // numero processeur: 13  
    free(registerFunc(finCompilation,       ";"));             // numero processeur: 14
    free(registerFunc(stringBegin,          "\""));            // numero processeur: 15 
    free(registerFunc(count,                "count"));         // numero processeur: 16   
    free(registerFunc(codeType,             "type"));          // numero processeur: 17  
    free(registerFunc(point,                "."));             // numero processeur: 18
    free(registerFunc(cr,                   "cr"));            // numero processeur: 19
    free(registerFunc(calculate,            "calculate"));     // numero processeur: 20       
    free(registerFunc(str,                  "str"));           // numero processeur: 21 
    free(registerFunc(diviser,              "/"));             // numero processeur: 22
    return;
}

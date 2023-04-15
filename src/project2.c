#include "anasynt.h"

int main (int argc, char** argv) {
    char buffer[] = "1/3+1/2";
    char *buf = buffer;
    if(argc == 2) {
        buf = argv[1];
    }
    arbre tree = AST(buf);
    printf("%s\n", buf);
    print_arbre(tree);

    printf("\nvalue: %d\n", calculateTree(tree));
    return 0;
}
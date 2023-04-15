#include "analex.h"

int main() {
    char *s;
    struct token *tokens;

    s = getfileall("./examples/factorielle.lac");
    tokens = tokenize(s);
    printTokens(tokens);

    return 0;
}
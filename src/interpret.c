#include "interpret.h"
#include <string.h>

void createTerminal() {
    char input[1024];
    printf("LAC interpreter information:\n");
    printf("Version: 0.1\n");
    printf("Author: nastyapple\n");
    printf("-> ");
    scanf("%[^\n]", input);
    interpretString(input);
    fflush(stdin);
    while(1) {
        printf("-> ");
        scanf("%[^\n]", input);
        if(strcmp(input, "exit") == 0) {return;}
        interpretString(input);
        fflush(stdin);
        memset(input, '\0', sizeof(input));
    }
    return;
}

void interpretString(const char *s) {
    compileString(s);
    run();
    return;
}


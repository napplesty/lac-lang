#include "interpret.h"

int main() {
    // essential components
    processeurInitialize();
    lacTableInitialize();
    vmTableInitialize();
    stackInitialize();
    registerInitialize();
    registerAllProcesseur();

    createTerminal();
    return 0;
}
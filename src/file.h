#include "global.h"
#include <stdio.h>

#ifndef FILE_H
#define FILE_H

// just restore vm and lac state is enough to execute the program
// and is easy to do that
void currentStateToFile(const char *fileName);
void fileToState(const char *fileName);

#endif
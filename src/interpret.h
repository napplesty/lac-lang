#include "common.h"
#include "global.h"
#include "processeur.h"
#include "analex.h"
#include "find.h"
#include "compilation.h"
#include "executeur.h"

#ifndef INTERPRET_H
#define INTERPRET_H

void createTerminal();
void interpretString(const char *s);

#endif

#ifndef LOGIC_MANAGER_H_INCLUDED
#define LOGIC_MANAGER_H_INCLUDED

#include <stdio.h>
#include "bool_func.h"

int getVarSet(int varCount, int index, char *set);
int fprintTruthTable(FILE *stream, const struct BoolFunc *func);
int fprintKarnaughMap(FILE *stream, const struct BoolFunc *func);

#endif // LOGIC_MANAGER_H_INCLUDED

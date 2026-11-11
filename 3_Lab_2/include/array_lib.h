#ifndef ARRAY_LIB_H_INCLUDED
#define ARRAY_LIB_H_INCLUDED

#include "bool_func.h"

char *allocFuncValues(int varCount);
int freeBoolFunc(struct BoolFunc *func);
int randomFillBoolFunc(struct BoolFunc *func);
char *append_char(char *arr, int *size, char value);
#endif // ARRAY_LIB_H_INCLUDED

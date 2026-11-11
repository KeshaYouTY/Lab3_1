#ifndef IO_LIB_H_INCLUDED
#define IO_LIB_H_INCLUDED

#include "../include/bool_func.h"
#include "../include/err_constants.h"
#include "../include/array_lib.h"
#include <stdio.h>
#include <stdlib.h>

int readIntLine(const char *line, int *out);
void clearInputBuffer(void);
int fillFuncFromFile(FILE *file, struct BoolFunc *func);
int readFuncFromKeyboard(struct BoolFunc *func);
#endif // IO_LIB_H_INCLUDED

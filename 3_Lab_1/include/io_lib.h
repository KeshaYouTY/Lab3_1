#ifndef IO_LIB_H_INCLUDED
#define IO_LIB_H_INCLUDED

#include "../include/matrix2D.h"
#include "../include/err_constants.h"
#include "../include/array_lib.h"
#include <stdio.h>
#include <stdlib.h>

int readIntLine(const char *line, int *out);
void clearInputBuffer(void);
int fprint2DSqrMatrix(FILE *stream, struct Matrix2D *matrix);
int fillMatrixFromFile(FILE *file, struct Matrix2D *matrix);
#endif // IO_LIB_H_INCLUDED

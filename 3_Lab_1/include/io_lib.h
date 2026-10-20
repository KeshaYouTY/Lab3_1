#ifndef IO_LIB_H_INCLUDED
#define IO_LIB_H_INCLUDED

#include "../include/usr_structs.h"
#include "../include/usr_constants.h"
#include "../include/array_lib.h"
#include <stdio.h>
#include <stdlib.h>

void clearStdin();
int fprint2DSqrMatrix(FILE *stream, struct Matrix2D *matrix);
int fillMatrixFromFile(FILE *file, struct Matrix2D *matrix);
#endif // IO_LIB_H_INCLUDED

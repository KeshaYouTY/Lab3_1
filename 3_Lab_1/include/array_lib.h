#ifndef ARRAY_LIB_H_INCLUDED
#define ARRAY_LIB_H_INCLUDED

int alloc2DSqrArray(int n, char*** array2D);
char **alloc2DSqrArrayV(int n);
int free2DSqrArray(int n, char** array2D);
int randomFill2DMatrix(struct Matrix2D *matrix);
char *append_char(char *arr, int *size, char value);
#endif // ARRAY_LIB_H_INCLUDED

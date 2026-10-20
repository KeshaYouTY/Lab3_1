#ifndef MESSANGER_H_INCLUDED
#define MESSANGER_H_INCLUDED
#include <stdio.h>

void ru_locale_init();
int askFileName(FILE **filePtr);
int askMatrixSize(int* size);
#endif // MESSANGER_H_INCLUDED

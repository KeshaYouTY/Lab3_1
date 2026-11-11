#ifndef NORMAL_FORMS_H_INCLUDED
#define NORMAL_FORMS_H_INCLUDED

#include <stdio.h>
#include "bool_func.h"

int fprintSDNF(FILE *stream, const struct BoolFunc *func);
int fprintSKNF(FILE *stream, const struct BoolFunc *func);

#endif // NORMAL_FORMS_H_INCLUDED

#ifndef BOOL_FUNC_H_INCLUDED
#define BOOL_FUNC_H_INCLUDED

struct BoolFunc {
    char *values;   // значения функции; индекс = номер набора (x1 - старший бит)
    int varCount;   // число переменных n, всего значений 2^n
};

#endif // BOOL_FUNC_H_INCLUDED

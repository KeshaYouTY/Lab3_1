#include <stdlib.h>
#include <time.h>
#include "../include/err_constants.h"
#include "../include/bool_func.h"
#include "../include/array_lib.h"

char *allocFuncValues(int varCount) {
    if(varCount < MIN_VAR_COUNT || varCount > MAX_VAR_COUNT) return NULL;

    return (char*)calloc((size_t)1 << varCount, sizeof(char));
}

int freeBoolFunc(struct BoolFunc *func) {
    if(func == NULL) return ERR_NULL_ARGUMENT;

    if(func->values != NULL) {
        free(func->values);
        func->values = NULL;
    }
    return 0;
}

int randomFillBoolFunc(struct BoolFunc *func) {
    if(func == NULL || func->values == NULL) {
        return ERR_NULL_ARGUMENT;
    }
    if(func->varCount < MIN_VAR_COUNT || func->varCount > MAX_VAR_COUNT) {
        return ERR_BAD_ARGUMENT;
    }

    static int seeded = 0;
    if(!seeded) {
        srand(time(NULL));
        seeded = 1;
    }

    for(int i = 0; i < (1 << func->varCount); ++i) {
        func->values[i] = rand() % 2;
    }

    return 0;
}

char *append_char(char *arr, int *size, char value) {
    char *tmp = realloc(arr, (*size + 1) * sizeof(char));
    if (tmp == NULL) {
        return NULL;
    }

    arr = tmp;
    arr[*size] = value;
    (*size)++;
    return arr;
}


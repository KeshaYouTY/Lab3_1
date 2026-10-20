#include <stdlib.h>
#include <time.h>
#include "../include/usr_constants.h"
#include "../include/usr_structs.h"
#include "../include/array_lib.h"
int alloc2DSqrArray(int n, char*** array2D) {
    if(array2D == NULL) return ERR_NULL_ARGUMENT;
    if(n <= 0) return ERR_BAD_ARGUMENT;

    *array2D = (char**)calloc(n, sizeof(char*));
    if(*array2D == NULL) return ERR_MEM_ALLOC;

    for(int i = 0; i < n; ++i) {
        (*array2D)[i] = (char*)calloc(n, sizeof(char));

        //Если происходит ошибка выделения памяти -> освободить уже занятую память.
        if((*array2D)[i] == NULL) {
            for(int j = 0; j < i; ++j) {
                free((*array2D)[j]);
            }
            free(*array2D);
            *array2D = NULL;
            return ERR_MEM_ALLOC;
        }
    }
    return 0;
}

char **alloc2DSqrArrayV(int n) {
    if(n <= 0) return NULL;

    char **array2D = (char**)calloc(n, sizeof(char*));
    if(array2D == NULL) return NULL;

    for(int i = 0; i < n; ++i) {
        array2D[i] = (char*)calloc(n, sizeof(char));

        //Если происходит ошибка выделения памяти -> освободить уже занятую память.
        if(array2D[i] == NULL) {
            for(int j = 0; j < i; ++j) {
                free(array2D[j]);
            }
            free(array2D);
            return NULL;
        }
    }
    return array2D;
}

int free2DSqrArray(int n, char** array2D) {
    if(array2D == NULL) return ERR_NULL_ARGUMENT;
    if(n <= 0) return ERR_BAD_ARGUMENT;

    for(int i = 0; i < n; ++i) {
        free(array2D[i]);
    }
    free(array2D);
    return 0;
}   //Вызывающий сам должен заNULLять ptr??

int randomFill2DMatrix(struct Matrix2D *matrix) {
    if(matrix == NULL || matrix->array == NULL) {
        return ERR_NULL_ARGUMENT;
    }
    if(matrix->size < 1) {
        return ERR_BAD_ARGUMENT;
    }

    static int srand_done = 0;
    if(!srand_done) {
        srand((unsigned)time(NULL));
        srand_done = 1;
    }

    for(int i = 0; i < matrix->size; ++i) {
        for(int j = i + 1; j < matrix->size; ++j) {
            matrix->array[i][j] = rand() % 2;
        }
    }
    for(int i = 0; i < matrix->size; ++i) {
        matrix->array[i][i] = 0;
        for(int j = 0; j < i; ++j) {
            matrix->array[i][j] = 0;
        }
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


#include "../include/DSM_manager.h"

bool isAntireflexive(struct Matrix2D *matrix) {
    if(matrix == NULL || matrix->array == NULL) {
        return false;
    }
    if(matrix->size < 1) {
        return false;
    }

    for(int i = 0; i < matrix->size; ++i) {
        if(matrix->array[i][i] == 1) {
            return false;
        }
    }
    return true;
}

bool isAntisymmetric(struct Matrix2D *matrix) {
    if(matrix == NULL || matrix->array == NULL) {
        return false;
    }
    if(matrix->size < 1) {
        return false;
    }

    for(int i = 1; i < matrix->size; ++i) {
        for(int j = 0; j < i; ++j) {
            if(matrix->array[i][j] == 1 &&
               matrix->array[j][i] == 1) {
                return false;
            }
        }
    }
    return true;
}

bool isTransitive(struct Matrix2D *matrix) {
    if(matrix == NULL || matrix->array == NULL) {
        return false;
    }
    if(matrix->size < 1) {
        return false;
    }

    for(int i = 0; i < matrix->size; ++i) {
        for(int j = 0; j < matrix->size; ++j) {
            for(int k = 0; k < matrix->size; ++k) {
                if(matrix->array[i][k] == 1 && matrix->array[k][j] == 1) {
                    if(matrix->array[i][j] == 0) {
                        return false;
                    }
                }
            }
        }
    }
    return true;
}

#include <stdlib.h>

#include "../include/DSM_manager.h"

#define MARK_NONE  0
#define MARK_TEMP  1
#define MARK_PERM  2

static int tarjanVisit(const struct Matrix2D *m, int v, char *mark,
                       int *order, int *pos) {
    if(mark[v] == MARK_PERM) return 0;
    if(mark[v] == MARK_TEMP) return ERR_CYCLE;

    mark[v] = MARK_TEMP;

    for(int to = 0; to < m->size; ++to) {
        if(m->array[v][to] == 1) {
            int err = tarjanVisit(m, to, mark, order, pos);
            if(err != 0) return err;
        }
    }

    mark[v] = MARK_PERM;
    order[--(*pos)] = v;
    return 0;
}

int topoSortTarjan(const struct Matrix2D *m, int *order) {
    if(m == NULL || m->array == NULL || order == NULL) return ERR_NULL_ARGUMENT;
    if(m->size < 1) return ERR_BAD_ARGUMENT;

    char *mark = (char*)calloc(m->size, sizeof(char));
    if(mark == NULL) return ERR_MEM_ALLOC;

    int pos = m->size;
    int err = 0;
    for(int v = 0; v < m->size && err == 0; ++v) {
        if(mark[v] == MARK_NONE) {
            err = tarjanVisit(m, v, mark, order, &pos);
        }
    }

    free(mark);
    return err;
}

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

bool isLinearOrder(struct Matrix2D *matrix) {
    if(matrix == NULL || matrix->array == NULL) {
        return false;
    }
    if(matrix->size < 1) {
        return false;
    }

    for(int i = 0; i < matrix->size; ++i) {
        for(int j = i + 1; j < matrix->size; ++j) {
            if(matrix->array[i][j] == 0 && matrix->array[j][i] == 0) {
                return false;
            }
        }
    }

    return true;
}

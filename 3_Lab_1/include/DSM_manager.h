#ifndef DSM_MANAGER_H_INCLUDED
#define DSM_MANAGER_H_INCLUDED

#include "matrix2D.h"
#include "err_constants.h"
#include <stdbool.h>
#include <stdlib.h>
bool isAntireflexive(struct Matrix2D *matrix);
bool isAntisymmetric(struct Matrix2D *matrix);
bool isTransitive(struct Matrix2D *matrix);
bool isLinearOrder(struct Matrix2D *matrix);
int topoSortTarjan(const struct Matrix2D *m, int *order);
#endif // DSM_MANAGER_H_INCLUDED

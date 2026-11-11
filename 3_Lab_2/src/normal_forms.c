#include <stdio.h>
#include "../include/err_constants.h"
#include "../include/bool_func.h"
#include "../include/logic_manager.h"
#include "../include/normal_forms.h"

#define NOT_SYM "!"
#define AND_SYM " & "
#define OR_SYM  " V "

// Печатает один терм: переменные x1..xn, где переменная с битом negateBit идёт с отрицанием,
// между ними innerOp
static void fprintTerm(FILE *stream, int n, const char *set,
                       int negateBit, const char *innerOp) {
    fprintf(stream, "(");
    for(int j = 0; j < n; ++j) {
        if(j > 0) {
            fprintf(stream, "%s", innerOp);
        }
        fprintf(stream, "%sx%d", set[j] == negateBit ? NOT_SYM : "", j + 1);
    }
    fprintf(stream, ")");
}

// Общая часть для СДНФ и СКНФ.
// targetValue: значение функции, по которому берутся наборы (1 для СДНФ, 0 для СКНФ).
static int fprintNormalForm(FILE *stream, const struct BoolFunc *func,
                            const char *name, int targetValue, int negateBit,
                            const char *innerOp, const char *outerOp) {
    if(stream == NULL || func == NULL || func->values == NULL) {
        return ERR_NULL_ARGUMENT;
    }
    if(func->varCount < MIN_VAR_COUNT || func->varCount > MAX_VAR_COUNT) {
        return ERR_BAD_ARGUMENT;
    }

    int n = func->varCount;
    int total = 1 << n;
    char set[MAX_VAR_COUNT];

    int count = 0;
    for(int i = 0; i < total; ++i) {
        if(func->values[i] == targetValue) ++count;
    }

    // Нет ни одного набора: СДНФ нет у константы 0, СКНФ нет у константы 1
    if(count == 0) {
        fprintf(stream, "%s не существует (функция тождественно равна %d).\n",
                name, 1 - targetValue);
        return 0;
    }

    fprintf(stream, "%s: f(", name);
    for(int j = 0; j < n; ++j) {
        fprintf(stream, "%sx%d", j > 0 ? "," : "", j + 1);
    }
    fprintf(stream, ") =\n");

    int printed = 0;
    for(int i = 0; i < total; ++i) {
        if(func->values[i] != targetValue) continue;

        int err = getVarSet(n, i, set);
        if(err != 0) return err;

        if(printed == 0) {
            fprintf(stream, "    ");
        } else {
            fprintf(stream, "%s ", outerOp);
        }
        fprintTerm(stream, n, set, negateBit, innerOp);
        fprintf(stream, "\n");
        ++printed;
    }
    return 0;
}

int fprintSDNF(FILE *stream, const struct BoolFunc *func) {
    // Наборы с f = 1, отрицание у переменных с 0, внутри &, между термами V
    return fprintNormalForm(stream, func, "СДНФ", 1, 0, AND_SYM, OR_SYM);
}

int fprintSKNF(FILE *stream, const struct BoolFunc *func) {
    // Наборы с f = 0, отрицание у переменных с 1, внутри V, между термами &
    return fprintNormalForm(stream, func, "СКНФ", 0, 1, OR_SYM, AND_SYM);
}

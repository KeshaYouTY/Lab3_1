#include <stdio.h>

#include "../include/err_constants.h"
#include "../include/bool_func.h"
#include "../include/logic_manager.h"

// Формирует набор значений переменных x1..xn по номеру строки таблицы.
// x1 - старший бит номера, поэтому наборы идут по возрастанию как двоичные числа.
// set должен вмещать не менее varCount элементов.
int getVarSet(int varCount, int index, char *set) {
    if(set == NULL) return ERR_NULL_ARGUMENT;
    if(varCount < MIN_VAR_COUNT || varCount > MAX_VAR_COUNT) return ERR_BAD_ARGUMENT;
    if(index < 0 || index >= (1 << varCount)) return ERR_BAD_ARGUMENT;

    for(int j = 0; j < varCount; ++j) {
        set[j] = (index >> (varCount - 1 - j)) & 1;
    }
    return 0;
}

// Выводит таблицу истинности в любой поток: stdout или файл.
int fprintTruthTable(FILE *stream, const struct BoolFunc *func) {
    if(stream == NULL || func == NULL || func->values == NULL) {
        return ERR_NULL_ARGUMENT;
    }
    if(func->varCount < MIN_VAR_COUNT || func->varCount > MAX_VAR_COUNT) {
        return ERR_BAD_ARGUMENT;
    }

    int n = func->varCount;
    char set[MAX_VAR_COUNT];

    // Шапка
    for(int j = 0; j < n; ++j) {
        fprintf(stream, "x%d ", j + 1);
    }
    fprintf(stream, "| f\n");

    // Разделитель
    for(int j = 0; j < n * 3; ++j) {
        fprintf(stream, "-");
    }
    fprintf(stream, "+--\n");

    // Строки таблицы
    for(int i = 0; i < (1 << n); ++i) {
        int err = getVarSet(n, i, set);
        if(err != 0) return err;

        for(int j = 0; j < n; ++j) {
            fprintf(stream, "%d  ", set[j]);
        }
        fprintf(stream, "| %d\n", func->values[i]);
    }
    return 0;
}

static int grayCode(int k) {
    return k ^ (k >> 1);
}

// Печатает младшие width бит числа (старший бит слева)
static void fprintBits(FILE *stream, int value, int width) {
    for(int b = width - 1; b >= 0; --b) {
        fprintf(stream, "%d", (value >> b) & 1);
    }
}

// Имена переменных печатаются как x1..x9, т.е. по 2 символа (хватает при MAX_VAR_COUNT <= 9)
int fprintKarnaughMap(FILE *stream, const struct BoolFunc *func) {
    if(stream == NULL || func == NULL || func->values == NULL) {
        return ERR_NULL_ARGUMENT;
    }
    if(func->varCount < MIN_VAR_COUNT || func->varCount > MAX_VAR_COUNT) {
        return ERR_BAD_ARGUMENT;
    }

    int n = func->varCount;
    int rowVars = n / 2;
    int colVars = n - rowVars;

    // подпись, какие переменные по строкам, какие по столбцам
    for(int j = 0; j < rowVars; ++j) fprintf(stream, "x%d", j + 1);
    fprintf(stream, "\\");
    for(int j = rowVars; j < n; ++j) fprintf(stream, "x%d", j + 1);
    fprintf(stream, "\n");

    // строка с кодами столбцов
    for(int k = 0; k < rowVars; ++k) fprintf(stream, " ");
    fprintf(stream, " | ");
    for(int c = 0; c < (1 << colVars); ++c) {
        fprintBits(stream, grayCode(c), colVars);
        fprintf(stream, " ");
    }
    fprintf(stream, "\n");

    // строки карты
    for(int r = 0; r < (1 << rowVars); ++r) {
        fprintBits(stream, grayCode(r), rowVars);
        fprintf(stream, " | ");
        for(int c = 0; c < (1 << colVars); ++c) {
            int index = (grayCode(r) << colVars) | grayCode(c);
            fprintf(stream, "%d", func->values[index]);
            for(int k = 0; k < colVars; ++k) fprintf(stream, " ");
        }
        fprintf(stream, "\n");
    }
    return 0;
}

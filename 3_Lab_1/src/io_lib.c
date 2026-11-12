#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>

#include "../include/io_lib.h"
#include "../include/DSM_manager.h"

int readIntLine(const char *line, int *out) {
    int i = 0;
    int numLen = 0;
    long long value = 0;
    int negative = 0;

    while(line[i] == ' ' || line[i] == '\t') ++i;

    if(line[i] == '-') {
        negative = 1;
        ++i;
    }

    while(line[i] >= '0' && line[i] <= '9') {
        value = value * 10 + (line[i] - '0');
        if(value > INT_MAX) {
            return 0;
        }
        ++i;
        ++numLen;
    }

    if(numLen == 0) return 0;

    while(line[i] == ' ' || line[i] == '\t') ++i;
    if(line[i] != '\0' && line[i] != '\r' && line[i] != '\n') return 0;

    if(negative) value = -value;
    *out = (int)value;

    return 1;
}

void clearInputBuffer(void) {
    int c;
    while((c = getchar()) != '\n' && c != EOF) { }
}

int fprint2DSqrMatrix(FILE *stream, struct Matrix2D *matrix) {
    if(stream == NULL || matrix == NULL || matrix->array == NULL) {
        return ERR_NULL_ARGUMENT;
    }
    if(matrix->size < 1) {
        return ERR_BAD_ARGUMENT;
    }

    for(int i = 0; i < matrix->size; ++i) {
        fprintf(stream, "%s", "   ");//"_______");
        for(int j = 0; j < matrix->size; ++j) {
            if(j > 0) {
                fprintf(stream, " ");
            }
            fprintf(stream, "%d", matrix->array[i][j]);
        }
        fprintf(stream, "\n");
    }
    return 0;
}

int fillMatrixFromFile(FILE *file, struct Matrix2D *matrix) {
    if(file == NULL || matrix == NULL) {
        return ERR_NULL_ARGUMENT;
    }

    freeMatrix2D(matrix);
    matrix->size = 0;

    //      1D
    int curVal;
    int line_array_size = 0;
    char *line_array = NULL;

        while(fscanf(file, "%d", &curVal) == 1) {

        if(line_array_size >= MAX_MATRIX_SIZE * MAX_MATRIX_SIZE) {
            printf("\nВ файле слишком много элементов (максимум %d x %d).\n",
                   MAX_MATRIX_SIZE, MAX_MATRIX_SIZE);
            free(line_array);
            return ERR_BAD_ARGUMENT;
        }

        if(curVal != 0 && curVal != 1) {
            printf("\nВ файле найден элемент '%d' (должно быть 0 или 1), он заменён на 0.\n", curVal);
            curVal = 0;
        }
        char *temp_ptr = append_char(line_array, &line_array_size, (char)curVal);

        if(temp_ptr == NULL) {  // realloc
            free(line_array);
            return ERR_MEM_ALLOC;
        }
        line_array = temp_ptr;
    }

    if(!feof(file)) {
        printf("\nВ файле встретился нечисловой символ.\n");
        free(line_array);
        return ERR_BAD_ARGUMENT;
    }

    if(line_array_size < 1) {
        free(line_array);
        return ERR_FILE_EMPTY;
    }


    {
        int size = 1;
        while(size * size < line_array_size) ++size;
        if(size * size != line_array_size) {
            printf("\nВ файле %d элементов, а должно быть точно квадратом"
                   " (матрица квадратная n x n). Ближайший подходящий размер: %d x %d = %d.\n",
                   line_array_size, size, size, size * size);
            free(line_array);
            return ERR_BAD_ARGUMENT;
        }
        matrix->size = size;
    }


    matrix->array = alloc2DSqrArrayV(matrix->size);
    if(matrix->array == NULL) {
        free(line_array);
        return ERR_MEM_ALLOC;
    }

    int k = 0;
    for(int i = 0; i < matrix->size; ++i) {
        for(int j = 0; j < matrix->size; ++j) {
            matrix->array[i][j] = line_array[k];
            ++k;
        }
    }
    free(line_array);
    return 0;

}

int fprintAnalysisResults(FILE *stream, struct Matrix2D *matrix) {
    if(stream == NULL || matrix == NULL || matrix->array == NULL) {
        return ERR_NULL_ARGUMENT;
    }
    if(matrix->size < 1) {
        return ERR_BAD_ARGUMENT;
    }

    bool isAR = isAntireflexive(matrix);
    bool isAS = isAntisymmetric(matrix);
    bool isTR = isTransitive(matrix);

    fprintf(stream, "Матрица антирефлексивна? : %s\n", isAR ? "да" : "нет");
    fprintf(stream, "Матрица антисимметрична? : %s\n", isAS ? "да" : "нет");
    fprintf(stream, "Матрица транзитивна?     : %s\n", isTR ? "да" : "нет");

    if(!(isAR && isAS && isTR)) {
        fprintf(stream, "\nОтношение не является строгим частичным порядком.\n"
                        "Регламент пуска построить нельзя, проверьте матрицу.\n");
        return 0;
    }

    bool isLinear = isLinearOrder(matrix);
    fprintf(stream, "Матрица связна (линейна)? : %s\n", isLinear ? "да" : "нет");

    if(isLinear) {
        fprintf(stream, "\nПорядок уже линейный, сортировка не требуется.\n"
                        "Единственный порядок пуска:\n");
    } else {
        fprintf(stream, "\nЕсть параллельные системы, необходима сортировка.\n");
    }

    int *order = (int*)malloc(matrix->size * sizeof(int));
    if(order == NULL) {
        return ERR_MEM_ALLOC;
    }

    int sortErr = topoSortTarjan(matrix, order);
    if(sortErr != 0) {
        free(order);
        return sortErr;
    }

    fprintf(stream, "\n===== РЕГЛАМЕНТ ПУСКА =====\n");
    for(int s = 0; s < matrix->size; ++s) {
        fprintf(stream, "Шаг %d: запустить систему x%d\n", s + 1, order[s] + 1);
    }
    fprintf(stream, "===========================\n");

    free(order);
    return 0;
}

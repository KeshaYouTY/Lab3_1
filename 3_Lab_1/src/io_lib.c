#include "../include/io_lib.h"
#include <ctype.h>

int readIntLine(const char *line, int *out) {
    int i = 0;
    int numLen = 0;

    while(line[i] == ' ' || line[i] == '\t') ++i;

    if(line[i] == '-') ++i;

    while(line[i] >= '0' && line[i] <= '9') {
        ++i;
        ++numLen;
    }

    if(numLen == 0) return 0;

    while(line[i] == ' ' || line[i] == '\t') ++i;
    if(line[i] != '\0' && line[i] != '\r' && line[i] != '\n') return 0;

    *out = 0;
    i = 0;
    while(line[i] != '\0') {
        if(line[i] >= '0' && line[i] <= '9') {
            *out = *out * 10 + (line[i] - '0');
        }
        ++i;
    }
    return 1;
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

    if(matrix->array != NULL) {
        free2DSqrArray(matrix->size, matrix->array);
        matrix->array = NULL;
    }
    matrix->size = 0;

    //      1D
    int curVal;
    int line_array_size = 0;
    char *line_array = NULL;

        while(fscanf(file, "%d", &curVal) == 1) {

        if(curVal != 0 && curVal != 1) {
            printf("\n!!! В файле найден элемент '%d' (должно быть 0 или 1), он заменён на 0.\n", curVal);
            curVal = 0;
        }
        char *temp_ptr = append_char(line_array, &line_array_size, (char)curVal);

        if(temp_ptr == NULL) {  // realloc
            free(line_array);
            return ERR_MEM_ALLOC;
        }
        line_array = temp_ptr;
    }

    if(line_array_size < 1) {
        free(line_array);
        return ERR_FILE_EMPTY;
    }


    // .   -    2D ,
    //matrix->size = ceil(sqrt(line_array_size));
    {
        int size = 1;
        while(size * size < line_array_size) ++size;
        if(size * size != line_array_size) {
            printf("\n!!! В файле %d элементов, а должно быть точно квадратом"
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

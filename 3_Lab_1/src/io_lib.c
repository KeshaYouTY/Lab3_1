#include "../include/io_lib.h"
#include <math.h>

void clearStdin() {
    int c;
    while((c = getchar()) != '\n' && c != EOF);
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
        //fprintf(stderr, "\n:  .");
        return ERR_NULL_ARGUMENT;
    }

    //free(matrix->array);
    matrix->array = NULL;
    matrix->size = 0;

    //      1D
    int curChar;
    int line_array_size = 1;
    char *line_array = (char*)calloc(1, sizeof(char));
    if(line_array == NULL) {
        return ERR_MEM_ALLOC;
    }


    if(fscanf(file, "%d", &curChar) != 1) {
        free(line_array);
        return ERR_FILE_EMPTY;
    }
    line_array[0] = ((curChar == 0 || curChar == 1) ? curChar : 0);

    //
    while(fscanf(file, "%d", &curChar) == 1) {
        char *temp_ptr = append_char(line_array, &line_array_size,(curChar == 0 || curChar == 1) ? curChar : 0);
        if(temp_ptr == NULL) {  // realloc  ,
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
        while (size * size < line_array_size) ++size;
        matrix->size = size;
    }
    //printf("\nSize: %d\nSize2D: %d", line_array_size, size2D);

    matrix->array = alloc2DSqrArrayV(matrix->size);
    if(matrix->array == NULL) {
        free(line_array);
        return ERR_MEM_ALLOC;
    }

    int k = 0;
    for(int i = 0; i < matrix->size; ++i) {
        if(!(k < line_array_size)) {
            break;
        }
        for(int j = 0; j < matrix->size; ++j) {
            if(!(k < line_array_size)) {
                break;
            }
            matrix->array[i][j] = line_array[k];
            ++k;
        }
    }
    free(line_array);
    return 0;

}

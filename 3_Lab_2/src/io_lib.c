#include <ctype.h>
#include <limits.h>

#include "../include/io_lib.h"
#include "../include/err_constants.h"
#include "../include/bool_func.h"
#include "../include/array_lib.h"

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

int fillFuncFromFile(FILE *file, struct BoolFunc *func) {
    if(file == NULL || func == NULL) {
        return ERR_NULL_ARGUMENT;
    }

    freeBoolFunc(func);
    func->varCount = 0;

    int curVal;
    int line_array_size = 0;
    char *line_array = NULL;

    while(fscanf(file, "%d", &curVal) == 1) {

        if(line_array_size >= (1 << MAX_VAR_COUNT)) {
            printf("\nВ файле слишком много значений (максимум %d, т.е. %d переменных).\n",
                   1 << MAX_VAR_COUNT, MAX_VAR_COUNT);
            free(line_array);
            return ERR_BAD_ARGUMENT;
        }

        if(curVal != 0 && curVal != 1) {
            printf("\nВ файле найден элемент '%d' (должно быть 0 или 1).\n", curVal);
            free(line_array);
            return ERR_BAD_ARGUMENT;
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
        int count = 0;
        while((1 << count) < line_array_size) ++count;
        if((1 << count) != line_array_size || count < MIN_VAR_COUNT) {
            printf("\nВ файле %d значений, а должно быть 2^n (n от %d до %d): 2, 4, 8, ...\n",
                   line_array_size, MIN_VAR_COUNT, MAX_VAR_COUNT);
            free(line_array);
            return ERR_BAD_ARGUMENT;
        }
        func->varCount = count;
    }

    func->values = line_array;
    return 0;
}

int readFuncFromKeyboard(struct BoolFunc *func) {
    if(func == NULL) {
        return ERR_NULL_ARGUMENT;
    }
    if(func->varCount < MIN_VAR_COUNT || func->varCount > MAX_VAR_COUNT) {
        return ERR_BAD_ARGUMENT;
    }

    freeBoolFunc(func);

    func->values = allocFuncValues(func->varCount);
    if(func->values == NULL) {
        return ERR_MEM_ALLOC;
    }

    int n = func->varCount;
    for(int i = 0; i < (1 << n); ++i) {
        int val = -1;
        int inputErrors = 0;
        while(val != 0 && val != 1) {
            printf("    f(");
            for(int j = 0; j < n; ++j) {
                printf("%s%d", j > 0 ? "," : "", (i >> (n - 1 - j)) & 1);
            }
            printf(") (0 или 1): ");

            char line[64];
            int parsed = -1;
            if(fgets(line, sizeof(line), stdin) == NULL) {
                printf("Консольный ввод закрыт (EOF).\n");
                freeBoolFunc(func);
                return EXC_INPUT_FAULT;
            }
            if(!readIntLine(line, &parsed)) {
                printf("Необходимо вводить 0 или 1! Повторите набор.\n");
            } else if(parsed != 0 && parsed != 1) {
                printf("Числа только 0 или 1! Повторите набор.\n");
            } else {
                val = parsed;
                continue;
            }

            ++inputErrors;
            if(inputErrors >= MAX_INPUT_ATTEMPTS) {
                printf("\nПревышено количество неверных попыток ввода.\n");
                freeBoolFunc(func);
                return EXC_INPUT_FAULT;
            }
        }
        func->values[i] = (char)val;
    }
    return 0;
}

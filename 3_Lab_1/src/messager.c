#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "../include/messanger.h"
#include "../include/io_lib.h"
#include "../include/usr_constants.h"

static int isAttemptsExceeded(int *inputErrors) {
    ++(*inputErrors);
    if(*inputErrors >= MAX_INPUT_ATTEMPTS) {
        printf("\nПревышено количество неверных попыток ввода.\n");
        return 1;
    }
    return 0;
}

int askFileName(FILE **filePtr) {
    int inputErrors = 0;
    while(1) {
        printf("Введите название файла (до %d симв.): ", MAX_FILENAME_LEN);
        char filename[MAX_FILENAME_LEN + 2];
        if(fgets(filename, sizeof(filename), stdin) == NULL) {
            printf("Консольный ввод закрыт (EOF).\n");
            *filePtr = NULL;
            return EXC_INPUT_FAULT;
        }

        if(strchr(filename, '\n') == NULL && !feof(stdin)) {
            clearInputBuffer();
            printf("Имя файла длиннее %d символов. Повторите попытку.\n", MAX_FILENAME_LEN);
            if(isAttemptsExceeded(&inputErrors)) {
                *filePtr = NULL;
                return EXC_INPUT_FAULT;
            }
            continue;
        }

        size_t len = strlen(filename);
        while(len > 0 && isspace((unsigned char)filename[len-1])) --len;
        filename[len] = '\0';
        if(len == 0) {
            printf("Имя файла пустое. Повторите попытку.\n");
            if(isAttemptsExceeded(&inputErrors)) {
                *filePtr = NULL;
                return EXC_INPUT_FAULT;
            }
            continue;
        }

        *filePtr = fopen(filename, "r");
        if(*filePtr == NULL) {
            printf("Такого файла не существует. Повторите попытку.\n");
            if(isAttemptsExceeded(&inputErrors)) {
                *filePtr = NULL;
                return EXC_INPUT_FAULT;
            }
            continue;
        }
        break;
    }
    printf("Файл найден.\n");
    return 0;
}

int askMatrixSize(int* size) {
    int inputErrors = 0;
    while(1) {
        printf("Введите размер матрицы: ");
        char line[64];
        if(fgets(line, sizeof(line), stdin) == NULL) {
            printf("Консольный ввод закрыт (EOF).\n");
            *size = 0;
            return EXC_INPUT_FAULT;
        }
        int parsed = 0;
        if(line[0] == '\0' || line[0] == '\n' || line[0] == '\r') {
            printf("Пустая строка. Повторите попытку.\n");
            if(isAttemptsExceeded(&inputErrors)) {
                *size = 0;
                return EXC_INPUT_FAULT;
            }
            continue;
        }
        if(readIntLine(line, &parsed)) {
            *size = parsed;
        } else {
            printf("Необходимо вводить целое число. Повторите попытку.\n");
            if(isAttemptsExceeded(&inputErrors)) {
                *size = 0;
                return EXC_INPUT_FAULT;
            }
            continue;
        }

        if(*size < 1 || *size > MAX_MATRIX_SIZE) {
            printf("Размер должен быть от 1 до %d. Повторите попытку.\n", MAX_MATRIX_SIZE);
            if(isAttemptsExceeded(&inputErrors)) {
                *size = 0;
                return EXC_INPUT_FAULT;
            }
            continue;
        }
        break;
    }
    return 0;
}





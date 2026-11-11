#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "../include/messanger.h"
#include "../include/io_lib.h"
#include "../include/err_constants.h"

static int isAttemptsExceeded(int *inputErrors) {
    ++(*inputErrors);
    if(*inputErrors >= MAX_INPUT_ATTEMPTS) {
        printf("\nПревышено количество неверных попыток ввода.\n");
        return 1;
    }
    return 0;
}

int askFileName(FILE **filePtr, const char *mode) {
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

        *filePtr = fopen(filename, mode);
        if(*filePtr == NULL) {
            if(mode[0] == 'r') {
                printf("Такого файла не существует. Повторите попытку.\n");
            } else {
                printf("Не удалось открыть файл для записи. Повторите попытку.\n");
            }
            if(isAttemptsExceeded(&inputErrors)) {
                *filePtr = NULL;
                return EXC_INPUT_FAULT;
            }
            continue;
        }
        break;
    }
    printf(mode[0] == 'r' ? "Файл найден.\n" : "Файл открыт для записи.\n");
    return 0;
}

int askVarCount(int* count) {
    int inputErrors = 0;
    while(1) {
        printf("Введите количество переменных (от %d до %d): ", MIN_VAR_COUNT, MAX_VAR_COUNT);
        char line[64];
        if(fgets(line, sizeof(line), stdin) == NULL) {
            printf("Консольный ввод закрыт (EOF).\n");
            *count = 0;
            return EXC_INPUT_FAULT;
        }
        int parsed = 0;
        if(line[0] == '\0' || line[0] == '\n' || line[0] == '\r') {
            printf("Пустая строка. Повторите попытку.\n");
            if(isAttemptsExceeded(&inputErrors)) {
                *count = 0;
                return EXC_INPUT_FAULT;
            }
            continue;
        }
        if(readIntLine(line, &parsed)) {
            *count = parsed;
        } else {
            printf("Необходимо вводить целое число. Повторите попытку.\n");
            if(isAttemptsExceeded(&inputErrors)) {
                *count = 0;
                return EXC_INPUT_FAULT;
            }
            continue;
        }

        if(*count < MIN_VAR_COUNT || *count > MAX_VAR_COUNT) {
            printf("Количество переменных должно быть от %d до %d. Повторите попытку.\n",
                   MIN_VAR_COUNT, MAX_VAR_COUNT);
            if(isAttemptsExceeded(&inputErrors)) {
                *count = 0;
                return EXC_INPUT_FAULT;
            }
            continue;
        }
        break;
    }
    return 0;
}





#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "../include/messanger.h"
#include "../include/io_lib.h"
#include "../include/usr_constants.h"


int inline askFileName(FILE **filePtr) {
    int inputErrors = 0;
    while(1) {
        printf("Введите найзвание файла (255 симв.): ");
        char filename[256];
        if(fgets(filename, sizeof(filename), stdin) == NULL) {
            printf("Консольный ввод закрыт (EOF).\n");
            *filePtr = NULL;
            return EXC_INPUT_FAULT;
        }
        size_t len = strlen(filename);
        while(len > 0 && isspace((unsigned char)filename[len-1])) --len;
        filename[len] = '\0';
        if(len == 0) {
            printf("Имя файла пустое. Повторите попытку.\n");
            continue;
        }

        //ОЧИСТКА БУФЕРА
        *filePtr = fopen(filename, "r");
        if(*filePtr == NULL) {
            ++inputErrors;
            if(inputErrors >= 3) {
                printf("Превышено количество неверных попыток ввода.");
                *filePtr = NULL;
                return EXC_INPUT_FAULT;
            }
            printf("Такого файла не существует. Повторите попытку.\n");
            continue;
        }
        break;
    }
    printf("Файл найден.\n");
    return 0;
}

int inline askMatrixSize(int* size) {
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
            continue;
        }
        if(readIntLine(line, &parsed)) {
            *size = parsed;
        } else {
            ++inputErrors;
            if(inputErrors >= 3) {
                printf("Превышено количество неверных попыток ввода.");
                *size = 0;
                return EXC_INPUT_FAULT;
            }
            printf("Необходимо вводить целое число. Повторите попытку.\n");
            continue;
        }

        if(*size < 1 || *size > MAX_MATRIX_SIZE) {
            ++inputErrors;
            if(inputErrors >= 3) {
                printf("Превышено количество неверных попыток ввода.");
                *size = 0;
                return EXC_INPUT_FAULT;
            }
            printf("Размер должен быть от 1 до %d. Повторите попытку.\n", MAX_MATRIX_SIZE);
            continue;
        }
        break;
    }
    return 0;
}





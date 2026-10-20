#include <locale.h>
#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>

#include "../include/messanger.h"
#include "../include/io_lib.h"
#include "../include/usr_constants.h"


void inline ru_locale_init() {
    setlocale(LC_ALL, "65001");
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
}

int inline askFileName(FILE **filePtr) {
    int inputErrors = 0;
    while(1) {
        printf("Введите найзвание файла (30 симв.): ");
        char filename[30];
        scanf("%29s", filename);
        clearStdin();
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
        if(scanf("%d", size) != 1) {
            clearStdin();
            ++inputErrors;
            if(inputErrors >= 3) {
                printf("Превышено количество неверных попыток ввода.");
                *size = 0;
                return EXC_INPUT_FAULT;
            }
            printf("Необходимо вводить целое число. Повторите попытку.\n");
            continue;
        }
        clearStdin();
        //ОЧИСТКА БУФЕРА

        if(*size < 1) {
            ++inputErrors;
            if(inputErrors >= 3) {
                printf("Превышено количество неверных попыток ввода.");
                *size = 0;
                return EXC_INPUT_FAULT;
            }
            printf("Размер должен быть положительным. Повторите попытку.\n");
            continue;
        }
        break;
    }
    return 0;
}





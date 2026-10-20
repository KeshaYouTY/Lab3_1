

// Заголовочные файлы
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <stdbool.h>

#include "../include/usr_constants.h"
#include "../include/usr_structs.h"
#include "../include/messanger.h"
#include "../include/io_lib.h"
#include "../include/array_lib.h"
#include "../include/DSM_manager.h"

int main()
{
// Преждевременные операции подготовки
    // Настройка кодировок
    ru_locale_init();

    //Объявление переменных и флагов
    int inputMethod = NOT_SET, err = 0;
    FILE* inStream = NULL;
    struct Matrix2D matrix;
    bool inLoop = true;

// Логика программы


    while (inLoop) {
        //------------------------------------ИСТОЧНИК ВВОДА------------------------------------
        // Выбор источника ввода пользователем
        printf("================================================================================\n"
               "=                       ИНИЦИАЛИЗАЦИЯ МАТРИЦЫ ЗАВИСИМОСТЕЙ                     =\n"
               "================================================================================\n\n");

        printf("Выберете тип источника:\n"
               "\t1 - Клавиатура\n"
               "\t2 - Файл\n"
               "\t3 - Случайно\n"
               "\t0 - Выйти\n"
               "Ваш выбор: ");
        scanf("%d", &inputMethod);
        clearStdin(); //ОЧИСТКА БУФЕРА
        printf("\n");

        //Switch-выражение по результатам выбора ввода
        switch(inputMethod) {
            //----------------------------------
            case KEYBOARD:  // Клавиатура
                inStream = stdin;
                err = askMatrixSize(&matrix.size);
                if(err != 0) {
                    return err;
                }
                printf("ARR SIZE: %d\n", matrix.size);
                break;
            //----------------------------------
            case READ_FILE: // Из файла
                err = askFileName(&inStream);
                if(err != 0) {
                    return err;
                }
                break;
            //----------------------------------
            case RANDOM: // Случайно
                err = askMatrixSize(&matrix.size);
                if(err != 0) {
                    return err;
                }
                printf("ARR SIZE: %d\n", matrix.size);
                break;
            //----------------------------------
            case NOT_SET: //----------ВЫХОД ИЗ ЦИКЛА ПРОГРАММЫ----------
                inLoop = false;
                continue;
            //----------------------------------
            default:
                printf("Такого варианта нет.\n");
                continue;
            //----------------------------------
        }
        //------------------------------------ПОЛУЧЕНИЕ ДАННЫХ / ЗАПОЛНЕНИЕ МАССИВА------------------------------------
        while(true) {

            if(inputMethod == KEYBOARD) {
                matrix.array = alloc2DSqrArrayV(matrix.size);
                if(matrix.array == NULL) {
                    return ERR_NULL_ARGUMENT;
                }

                for(int i = 0; i < matrix.size; ++i) {
                    for(int j = 0; j < matrix.size; ++j) {
                        printf("Введите элемент под индексом [%d][%d]: ", i, j);
                        int val;
                        scanf("%d", &val);
                        matrix.array[i][j] = (char)val;
                        clearStdin();
                    }
                }

            } else if(inputMethod == READ_FILE) {
//                char *array = (char*)calloc(1, sizeof(char));
//                if(array == NULL) {
//                    return ERR_MEM_ALLOC;
//                }

                if((err = fillMatrixFromFile(inStream, &matrix)) != 0) {
                    printf("%d", err);
                    return err;
                }
                //return 100;

            } else if(inputMethod == RANDOM) {
                matrix.array = alloc2DSqrArrayV(matrix.size);
                if(matrix.array == NULL) {
                    return ERR_NULL_ARGUMENT;
                }

                err = randomFill2DMatrix(&matrix);
                if(err != 0) {
                    return err;
                }
            } else {
                return -100;
            }

            if(matrix.array != NULL) {
                printf("\nВведённая матрица:\n");
                fprint2DSqrMatrix(stdout, &matrix);
            } else {
                printf("NULL");
            }

            break;
        }
        if(inStream != stdin) {
            fclose(inStream);
        }
        printf("\nПродолжить выполнение:\n");
        _getch();
        if (matrix.array != NULL) {
            free2DSqrArray(matrix.size, matrix.array);
            matrix.array = NULL;
        }
        //system("cls");
        //------------------------------------ Задание ------------------------------------

        printf("Матрица антирефлексивна? : %d\n", isAntireflexive(&matrix));
        printf("Матрица антисимметрична? : %d\n", isAntisymmetric(&matrix));
        printf("Матрица транзитивна? : %d\n", isTransitive(&matrix));







        printf("(Конец программы)\n");
        _getch();
        system("cls");
        //break;
    }

    return 0;
}


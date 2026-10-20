

// Заголовочные файлы
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <Windows.h>
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
    setlocale(LC_ALL, "Russian");
    setlocale(LC_NUMERIC, "C");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);


    //Объявление переменных и флагов
    int inputMethod = NOT_SET, err = 0;
    FILE* inStream = NULL;
    struct Matrix2D matrix = { NULL, 0 }; ;
    matrix.array = NULL;
    matrix.size = 0;
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
        {
            char line[64];
            int parsed = 0;
            if(fgets(line, sizeof(line), stdin) == NULL) {
                printf("Консольный ввод закрыт (EOF).\n");
                return EXC_INPUT_FAULT;
            }
            if(readIntLine(line, &parsed)) {
                inputMethod = parsed;
            } else {
                printf("Необходимо вводить число от 0 до 3.\n");
                continue;
            }
        }

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
                printf("Размер матрицы: %d\n", matrix.size);
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
                printf("Размер матрицы: %d\n", matrix.size);
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
                if(matrix.array != NULL) {
                    free2DSqrArray(matrix.size, matrix.array);
                    matrix.array = NULL;
                }

                matrix.array = alloc2DSqrArrayV(matrix.size);
                if(matrix.array == NULL) {
                    return ERR_MEM_ALLOC;
                }

                for(int i = 0; i < matrix.size; ++i) {
                    for(int j = 0; j < matrix.size; ++j) {
                        int val = -1;
                        while(val != 0 && val != 1) {
                            printf("    [%d][%d] (0 или 1): ", i, j);
                            char line[64];
                            int parsed = -1;
                            if(fgets(line, sizeof(line), stdin) == NULL) {
                                printf("Консольный ввод закрыт (EOF).\n");
                                free2DSqrArray(matrix.size, matrix.array);
                                matrix.array = NULL;
                                return EXC_INPUT_FAULT;
                            }
                            if(!readIntLine(line, &parsed)) {
                                printf("Надо вводить 0 или 1! Повторите элемент [%d][%d].\n", i, j);
                                continue;
                            }
                            val = parsed;
                            if(val != 0 && val != 1) {
                                printf("Числа только 0 или 1! Повторите элемент [%d][%d].\n", i, j);
                            }
                        }
                        matrix.array[i][j] = (char)val;
                    }
                }

            } else if(inputMethod == READ_FILE) {
//                char *array = (char*)calloc(1, sizeof(char));
//                if(array == NULL) {
//                    return ERR_MEM_ALLOC;
//                }

                if((err = fillMatrixFromFile(inStream, &matrix)) != 0) {
                    printf("Ошибка загрузки матрицы из файла: код %d\n", err);
                    fclose(inStream);
                    inStream = NULL;
                    break;
                }
                //return 100;

            } else if(inputMethod == RANDOM) {
                if(matrix.array != NULL) {
                    free2DSqrArray(matrix.size, matrix.array);
                    matrix.array = NULL;
                }

                matrix.array = alloc2DSqrArrayV(matrix.size);

                if(matrix.array == NULL) {
                    return ERR_MEM_ALLOC;
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

        if(inStream != NULL && inStream != stdin) {
            fclose(inStream);
        }

        inStream = NULL;
        printf("\nПродолжить выполнение:\n");
        _getch();

       /* bool ar = isAntireflexive(&matrix);
        bool as = isAntisymmetric(&matrix);
        bool tr = isTransitive(&matrix);
        bool lin = isLinearOrder(&matrix);

        printf(" ? : %s\n", ar ? "Да" : "Нет");
        printf(" ? : %s\n", as ? "Да" : "Нет");
        printf(" ?     : %s\n", tr ? "Да" : "Нет");
        printf("  (   )? : %s\n", lin ? "Да" : "Нет");
        if(ar && as && tr) {
            printf("Матрица задаёт отношение строгого математического порядка.\n");
        } else {
            printf("Матрица НЕ задаёт конкретного отношения порядка!\n");
        }*/

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
    }

    return 0;
}


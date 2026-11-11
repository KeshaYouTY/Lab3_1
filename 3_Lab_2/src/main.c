#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <Windows.h>
#include <conio.h>
#include <stdbool.h>

#include "../include/err_constants.h"
#include "../include/bool_func.h"
#include "../include/messanger.h"
#include "../include/io_lib.h"
#include "../include/array_lib.h"
#include "../include/normal_forms.h"
#include "../include/logic_manager.h"

int main()
{
// Преждевременные операции подготовки
    // Настройка кодировок
    setlocale(LC_ALL, "Russian");
    setlocale(LC_NUMERIC, "C");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);


    //Объявление переменных и флагов
    int inputMethod = -1, err = 0;
    int menuErrors = 0;
    FILE* inStream = NULL;
    FILE* outStream = NULL;
    struct BoolFunc func = { NULL, 0 };
    bool inLoop = true;

// Логика программы


    while (inLoop) {
        // Выбор источника ввода пользователем
        printf("================================================================================\n"
               "=                 ПОСТРОЕНИЕ СДНФ И СКНФ ПО ТАБЛИЦЕ ИСТИННОСТИ                 =\n"
               "================================================================================\n\n");

        printf("Выберите тип источника значений функции:\n"
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
            if(!readIntLine(line, &parsed) || parsed < 0 || parsed > 3) {
                printf("Необходимо вводить число от 0 до 3.\n");
                ++menuErrors;
                if(menuErrors >= MAX_INPUT_ATTEMPTS) {
                    printf("\nПревышено количество неверных попыток ввода.\n");
                    return EXC_INPUT_FAULT;
                }
                continue;
            }
            menuErrors = 0;
            inputMethod = parsed;
        }

        printf("\n");

        //Switch-выражение по результатам выбора ввода
        switch(inputMethod) {

            case KEYBOARD:  // Клавиатура
                inStream = stdin;
                err = askVarCount(&func.varCount);
                if(err != 0) {
                    return err;
                }
                printf("Количество переменных: %d\n", func.varCount);
                break;

            case READ_FILE: // Из файла
                err = askFileName(&inStream, "r");
                if(err != 0) {
                    return err;
                }
                break;

            case RANDOM: // Случайно
                err = askVarCount(&func.varCount);
                if(err != 0) {
                    return err;
                }
                printf("Количество переменных: %d\n", func.varCount);
                break;

            case EXIT_PROGRAM: //ВЫХОД ИЗ ЦИКЛА ПРОГРАММЫ
                inLoop = false;
                continue;

            default:
                printf("Такого варианта нет.\n");
                continue;
        }
        //ПОЛУЧЕНИЕ ДАННЫХ / ЗАПОЛНЕНИЕ ВЕКТОРА ФУНКЦИИ
        while(true) {

            if(inputMethod == KEYBOARD) {
                printf("\nВведите значения функции для каждого набора переменных:\n");

                if((err = readFuncFromKeyboard(&func)) != 0) {
                    return err;
                }

            } else if(inputMethod == READ_FILE) {

                if((err = fillFuncFromFile(inStream, &func)) != 0) {
                    printf("Ошибка загрузки функции из файла: код %d\n", err);
                    fclose(inStream);
                    inStream = NULL;
                    break;
                }
                printf("Количество переменных (по содержимому файла): %d\n", func.varCount);

            } else if(inputMethod == RANDOM) {
                freeBoolFunc(&func);

                func.values = allocFuncValues(func.varCount);

                if(func.values == NULL) {
                    return ERR_MEM_ALLOC;
                }

                err = randomFillBoolFunc(&func);
                if(err != 0) {
                    return err;
                }
            } else {
                return ERR_INTERNAL;
            }

            break;
        }

        if(inStream != NULL && inStream != stdin) {
            fclose(inStream);
        }

        inStream = NULL;
        if(func.values == NULL) {
            printf("Функция не задана. Пропуск анализа.\n");
            continue;
        }

        //ФАЙЛ ДЛЯ РЕЗУЛЬТАТОВ
        // Выходной файл запрашивается после закрытия входного,
        // чтобы случайно не затереть входные данные
        printf("\nФайл для сохранения результатов.\n");
        err = askFileName(&outStream, "w");
        if(err != 0) {
            freeBoolFunc(&func);
            return err;
        }

        //Задание
        // Одинаковый вывод на экран и в файл
        FILE *streams[2] = { stdout, outStream };
        for(int s = 0; s < 2; ++s) {
            FILE *out = streams[s];

            fprintf(out, "\n===== ТАБЛИЦА ИСТИННОСТИ =====\n");
            err = fprintTruthTable(out, &func);
            if(err != 0) break;

            fprintf(out, "\n===== КАРТА КАРНО =====\n");
            err = fprintKarnaughMap(out, &func);
            if(err != 0) break;

            fprintf(out, "\n===== СОВЕРШЕННЫЕ НОРМАЛЬНЫЕ ФОРМЫ =====\n");
            err = fprintSDNF(out, &func);
            if(err != 0) break;

            fprintf(out, "\n");
            err = fprintSKNF(out, &func);
            if(err != 0) break;
        }

        if(err != 0) {
            printf("\nОшибка вывода результатов: код %d.\n", err);
        } else {
            printf("\nРезультаты записаны в файл.\n");
        }

        fclose(outStream);
        outStream = NULL;

        freeBoolFunc(&func);
        func.varCount = 0;


        printf("\nКонец программы.\n");
        printf("Нажмите любую кнопку для продолжения...");
        _getch();
        system("cls");
    }

    return 0;
}

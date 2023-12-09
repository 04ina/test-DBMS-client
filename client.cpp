#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <conio.h>

#include "client.h"

int main(void) {
    setlocale(LC_ALL, "Rus");

    relslottype relslot[SLOTCOUNT];
    memset(relslot, 0x00, SLOTCOUNT * sizeof(relslottype));
    int status = IS_SLOTCHOICE;
    int curslot;
    char choice;

    while (1) {
        system("cls");
        slotprint(relslot);

        switch (status) {
        case IS_SLOTCHOICE:
            do {
                printf("Выберите слот: (для выхода введите - \'e\')\n");
                choice = _getch();
                if ('0' <= choice && choice <= '7') {
                    curslot = choice - '0';
                    if (relslot[curslot].used) {
                        status = IS_SLOTUSED;
                    }
                    else {
                        status = IS_SLOTEMPTY;
                    }
                }
                else if (choice == 'e') {
                    status = IS_EXIT;
                }
                else {
                    printf("Команда не распознана\n");
                    choice = 1;
                }
            } while (choice == 1);
            break;

        case IS_SLOTEMPTY:
            printf("Выбранный слот: %d\n", curslot);
            printf("Доступные операции:\n");
            printf("1. Чтение таблицы из файла\n");
            printf("2. Соединение таблиц по столбцу и запись результирующей в слот\n");
            printf("3. Создание шаблона\n");
            printf("4. Копия таблицы\n");
            do {
                choice = _getch();
                switch (choice) {
                case '1':
                    readtable_menu(relslot, curslot);
                    status = IS_SLOTUSED;
                    break;
                case '2':
                    status = join_menu(relslot, curslot);
                    break;
                case '3':
                    status = createtemplate_menu(relslot, curslot);
                    break;
                case '4':
                    status = cpytable_menu(relslot, curslot);
                    break;
                case 'e':
                    status = IS_SLOTCHOICE;
                    break;
                default:
                    printf("Команда не распознана\n");
                    choice = 1;
                    break;
                }

            } while (choice == 1);
            break;
        case IS_SLOTUSED:
            printf("Выбранный слот: %d\n", curslot);
            printf("Доступные операции: (Для выхода введите - \'e\')\n");
            printf("1. Вывод таблицы\n");
            printf("2. Выборка\n"); // с целочисленными значениями // с массивами символов
            printf("3. Сортировка по столбцу с целочисленными значениями\n");
            printf("4. Проекция\n");
            printf("5. Группировка с тремя агрегатными функциями: avg, sum, count\n");
            printf("6. Вставка строки\n");
            printf("7. Очистка слота\n");
            printf("8. Запись таблицы в файл\n");
            printf("9. Вывод информации о таблице\n");
            do {
                choice = _getch();
                switch (choice) {
                case '1':
                    system("cls");
                    relprint(relslot[curslot].r);
                    _getch();
                    break;
                case '2':
                    status = where_menu(relslot, curslot);
                    break;
                case '3':
                    status = sort_menu(relslot, curslot);
                    break;
                case '4':
                    status = projection_menu(relslot, curslot);
                    break;
                case '5':
                    status = group_menu(relslot, curslot);
                    break;
                case '6':
                    status = gettuple_menu(relslot, curslot);
                    break;
                case '7':
                    status = freeslot(relslot, curslot);
                    break;
                case '8':
                    status = writetable_menu(relslot, curslot);
                    break;
                case '9':
                    system("cls");
                    relinfoprint(relslot[curslot].r);
                    _getch();
                    break;
                case 'e':
                    status = IS_SLOTCHOICE;
                    break;
                default:
                    printf("Команда не распознана\n");
                    choice = 1;
                    break;
                }

            } while (choice == 1);
            break;
        case IS_EXIT:
            return 0;
            break;

        }
    }
}

char *getarrchar(int size) {
    char* arrchar = (char*)calloc(size, sizeof(char));
    int quote = 1;
    char c;
    c = getc(stdin);
    if (c == '"') {
        for (int k = 0; k < size; k++) {
            c = getc(stdin);
            if (c == '"') {
                quote--;
                break;
            }
            if (-128 <= c && c <= -81) {
                c += 320;
            } else if (-32 <= c && c <= -17) {
                c += 272;
            }
            arrchar[k] = c;
        }
    } else {
        printf("Неправильный ввод.Строка должна быть в двойных кавычках, а max size: %d", size);
        return NULL;
    }

    if (quote) {
        c = getc(stdin);
        if (c == '"') { 
            return arrchar;
        } else {
            printf("Неправильный ввод.Строка должна быть в двойных кавычках, а max size: %d", size);
        }
    } else {
        return arrchar;
    }
    return NULL;
}

rel* init_students(void) {
    int Natt = 4;
    relatt* att = (relatt*)malloc(sizeof(relatt) * Natt);

    att[0].name = (char*)malloc(strlen("id студента")+1);
    strcpy(att[0].name, "id студента");
    att[0].type = INT_32;
    att[0].size = 4;

    att[1].name = (char*)malloc(strlen("Полное имя") + 1);
    strcpy(att[1].name, "Полное имя");
    att[1].type = ARRCHAR;
    att[1].size = 25;

    att[2].name = (char*)malloc(strlen("факультет") + 1);
    strcpy(att[2].name, "факультет");
    att[2].type = ARRCHAR;
    att[2].size = 7;

    att[3].name = (char*)malloc(strlen("Группа") + 1);
    strcpy(att[3].name, "Группа");
    att[3].type = ARRCHAR;
    att[3].size = 7;

    rel* r = relinit(att, Natt);

    return r;
}

rel* init_grades(void) {
    int Natt = 6;
    relatt* att = (relatt*)malloc(sizeof(relatt) * Natt);

    att[0].name = (char*)malloc(strlen("id оценки") + 1);
    strcpy(att[0].name, "id оценки");
    att[0].type = INT_32;
    att[0].size = 4;

    att[1].name = (char*)malloc(strlen("id студента") + 1);
    strcpy(att[1].name, "id студента");
    att[1].type = INT_32;
    att[1].size = 4;

    att[2].name = (char*)malloc(strlen("дисциплина") + 1);
    strcpy(att[2].name, "дисциплина");
    att[2].type = ARRCHAR;
    att[2].size = 30;

    att[3].name = (char*)malloc(strlen("Оценка") + 1);
    strcpy(att[3].name, "Оценка");
    att[3].type = INT_32;
    att[3].size = 4;

    att[4].name = (char*)malloc(strlen("Вид работы") + 1);
    strcpy(att[4].name, "Вид работы");
    att[4].type = ARRCHAR;
    att[4].size = 10;

    att[5].name = (char*)malloc(strlen("Семестр") + 1);
    strcpy(att[5].name, "Семестр");
    att[5].type = INT_32;
    att[5].size = 4;

    rel* r = relinit(att, Natt);

    return r;
}

void slotprint(relslottype* relslot) {
    printf("Слоты:\n");
    for (int i = 0; i < SLOTCOUNT; i++) {
        printf("слот %d: %s\n", i, relslot[i].used == 1 ? "используется" : "пустой");
    }
}

int freeslot(relslottype* relslot, int curslot) {
    delrel(relslot[curslot].r);
    relslot[curslot].used = 0;
    return IS_SLOTEMPTY;
}

int projection_menu(relslottype* relslot, int curslot) {
    system("cls");
    relinfoprint(relslot[curslot].r);
    int Natt;
    int* attarg = NULL;

    printf("Проекция:\n");
    while (1) {
        printf("Введите количество столбцов, которые нужно спроецировать:\n");
        scanf("%d", &Natt);
        if (Natt > relslot[curslot].r->Natt || Natt < 0) {
            printf("Недопустимое значение");
        } else {
            break;
        }
    }

    attarg = (int*)malloc(Natt * sizeof(int));

    printf("Введите номера столбцов, которые нужно спроецировать:\n");
    for (int i = 0; i < Natt; i++) {
        while (1) {
            scanf("%d", &attarg[i]);
            if (attarg[i] < 0 || attarg[i] > relslot[curslot].r->Natt - 1) {
                printf("Столбца \"%d\" не существует\n", attarg[i]);
            } else {
                break;
            }
        }
    }
    for (int i = 0; i < Natt; i++) {
        printf("%d ", attarg[i]);
    }

    projectionrel(&(relslot[curslot].r), attarg, Natt);

    return IS_SLOTUSED;
}

int where_menu(relslottype* relslot, int curslot) {
    system("cls");
    relinfoprint(relslot[curslot].r);
    char choice;
    int att;
    char cond;
    int val_int32;
    char *val_arrchar = NULL;

    printf("Выборка: (Для выхода введите - \'e\')\n");
    while (1) {
        printf("Введите номер столбца, по которому будет происходить выборка:\n");
        att = _getch() - '0';
        if (att < 0 || att > relslot[curslot].r->Natt - 1) {
            printf("Такого столбца нет\n");
            _getch();
            return IS_SLOTUSED;
        } else {
            break;
        }
    } 
    if (relslot[curslot].r->att[att].type == INT_32) {

        printf("Введите условие ('=', '!', '>', '<'):\n");
        cond = _getch();
        if (cond != '=' && cond != '!' && cond != '>' && cond != '<') {
            printf("Такого условия нет\n");
            _getch();
            return IS_SLOTUSED;
        } 

        printf("Введите значение для сравнения:\n");
        scanf("%d", &val_int32);

        whererel_int32(&(relslot[curslot].r), cond, att, val_int32);
    }
    else if (relslot[curslot].r->att[att].type == ARRCHAR) {

        printf("Введите условие ('=', '!'):\n");
        cond = _getch();
        if (cond != '=' && cond != '!') {
            printf("Такого условия нет\n");
            _getch();
            return IS_SLOTUSED;
        }

        printf("Введите значение для сравнения:\n");
        do {
            val_arrchar = getarrchar(relslot[curslot].r->att[att].size);
        } while (val_arrchar == NULL);

        whererel_arrchar(&(relslot[curslot].r), cond, att, val_arrchar);
    } else {
        printf("По этому столбцу нельзя делать выборку");
    }

    return IS_SLOTUSED;
}

int createtemplate_menu(relslottype* relslot, int curslot) {
    system("cls");
    slotprint(relslot);

    int table;

    printf("Создание шаблона:\n");
    printf("Введите номер слота, по которому будет создан шаблон:\n");
    scanf("%d", &table);
    if (table > SLOTCOUNT || table < 0) {
        printf("Такого слота нет\n");
        _getch();
        return IS_SLOTEMPTY;
    } else if (relslot[table].used == 0) {
        printf("Этот слот пуст\n");
        _getch();
        return IS_SLOTEMPTY;
    }

    relslot[curslot].r = relcpy_nodata(relslot[table].r);
    relslot[curslot].used = 1;
    return IS_SLOTUSED;
}

int cpytable_menu(relslottype* relslot, int curslot) {
    system("cls");
    slotprint(relslot);

    int table;

    printf("Копия таблицы:\n");
    printf("Введите номер слота, который будет продублирован:\n");
    scanf("%d", &table);
    if (table > SLOTCOUNT || table < 0) {
        printf("Такого слота нет\n");
        _getch();
        return IS_SLOTEMPTY;
    }
    else if (relslot[table].used == 0) {
        printf("Этот слот пуст\n");
        _getch();
        return IS_SLOTEMPTY;
    }

    relslot[curslot].r = relcpy(relslot[table].r);
    relslot[curslot].used = 1;
    return IS_SLOTUSED;
}

int join_menu(relslottype* relslot, int curslot) {
    system("cls");
    slotprint(relslot);
    rel* r = NULL;
    int table1, table2;
    int att1, att2;

    printf("Соединение двух таблиц:\n");

    printf("Введите номер первой таблицы\n");
    scanf("%d", &table1);
    if (table1 > SLOTCOUNT || table1 < 0) {
        printf("Такого слота нет\n");
        _getch();
        return IS_SLOTEMPTY;
    } else if (relslot[table1].used == 0) {
        printf("Этот слот пуст\n");
        _getch();
        return IS_SLOTEMPTY;
    }

    printf("Введите номер столбца первой таблицы, по которой будет происходить соединение: (нумерация начинается с нуля)\n");
    scanf("%d", &att1);
    if ((att1 > relslot[table1].r->Natt - 1 || att1 < 0)) {
        printf("Такого столбца нет\n");
        _getch();
        return IS_SLOTEMPTY;
    }

    printf("Введите номер второй таблицы\n");
    scanf("%d", &table2);
    if (table2 > SLOTCOUNT || table2 < 0) {
        printf("Такого слота нет\n");
        _getch();
        return IS_SLOTEMPTY;
    } else if (relslot[table2].used == 0) {
        printf("Этот слот пуст\n");
        _getch();
        return IS_SLOTEMPTY;
    }
    
    printf("Введите номер столбца второй таблицы, по которой будет происходить соединение: (нумерация начинается с нуля)\n");
    scanf("%d", &att2);
    if ((att2 > relslot[table1].r->Natt - 1 || att2 < 0)) {
        printf("Такого столбца нет\n");
        _getch();
        return IS_SLOTEMPTY;
    }

    if (relslot[table1].r->att[att1].type == INT_32 && relslot[table2].r->att[att2].type == INT_32) {
        r = joinrel_int32(relslot[table1].r, att1, relslot[table2].r, att2);    
    } else if (relslot[table1].r->att[att1].type == ARRCHAR && relslot[table2].r->att[att2].type == ARRCHAR) {
        r = joinrel_arrchar(relslot[table1].r, att1, relslot[table2].r, att2);
    } else {
        printf("Можно соединять таблицы либо по столбцам с целочисленными значениями, либо по столбцам с массивами символов");
        return IS_SLOTEMPTY;
    }
    
    
    relslot[curslot].r = r;
    relslot[curslot].used = 1;
    return IS_SLOTUSED;
}

int writetable_menu(relslottype* relslot, int curslot) {
    system("cls");
    char choice;
        printf("Таблицы, доступные для записи:\n");
        printf("1. Студенты:\n");
        printf("2. Оценки\n");
        choice = _getch();
        switch (choice) {
        case '1':
            writerel(relslot[curslot].r, "students");
            break;
        case '2':
            writerel(relslot[curslot].r, "grades");
            break;
        default:
            printf("Команда не распознана\n");
            _getch();
            return IS_SLOTUSED;
        }

    return IS_SLOTUSED;
}

int readtable_menu(relslottype* relslot, int curslot) {
    system("cls");
    char choice;
    rel* r = NULL;
    printf("Таблицы, доступные для чтения:\n");
    printf("1. Студенты:\n");
    printf("2. Оценки\n");
    while (1) {
        choice = _getch();
        if (choice == '1') {
            relslot[curslot].r = init_students();
            readrel(relslot[curslot].r, "students");
            break;
        } else if (choice == '2') {
            relslot[curslot].r = init_grades();
            readrel(relslot[curslot].r, "grades");
            break;
        } else {
            printf("Команда не распознана\n");
        }
    } 
    relslot[curslot].used = 1;
    return IS_SLOTUSED;
}

int sort_menu(relslottype* relslot, int curslot) {
    system("cls");
    relinfoprint(relslot[curslot].r);
    int att;
    int sorttype;

    printf("Введите номер столбца таблицы, по которой будет происходить сортировка:\n");
    scanf("%d", &att);
    if ((att > relslot[curslot].r->Natt - 1 || att < 0)) {
        printf("Такого столбца нет\n");
        _getch();
        return IS_SLOTEMPTY;
    }

    printf("Введите тип сортировки:\n");
    printf("1. По возрастанию.\n");
    printf("2. По убыванию.\n");
    scanf("%d", &sorttype);
    if (sorttype != INCREASE && sorttype != DECREASE) {
        printf("Команда не распознана\n");
        _getch();
        return IS_SLOTEMPTY;
    } 

    sortrel_int32(relslot[curslot].r, att, sorttype);

    return IS_SLOTUSED;
}

int group_menu(relslottype* relslot, int curslot) {
    system("cls");
    relinfoprint(relslot[curslot].r);
    int groupatt;
    int aggatt;
    int aggtype;

    printf("Введите номер столбца, по которому будет проведена группировка\n");
    scanf("%d", &groupatt);
    if (groupatt < 0 || groupatt > relslot[curslot].r->Natt - 1) {
        printf("Столбца \"%d\" не существует\n", groupatt);
        _getch();
        return IS_SLOTUSED;
    } else if (relslot[curslot].r->att[groupatt].type != ARRCHAR) {
        printf("Группировка должна быть по столбцу с типом данных ARRCHAR\n");
        _getch();
        return IS_SLOTUSED;
    }

    printf("Введите номер столбца, по которому будет проведено агрегирование\n");
    scanf("%d", &aggatt);
    if (aggatt < 0 || aggatt > relslot[curslot].r->Natt - 1) {
        printf("Столбца \"%d\" не существует\n", aggatt);
        _getch();
        return IS_SLOTUSED;
    } else if (relslot[curslot].r->att[aggatt].type != INT_32) {
        printf("агрегация должна быть по столбцу с типом данных INT_32\n");
        _getch();
        return IS_SLOTUSED;
    } 

    printf("Выберите режим агрегирования:\n");
    printf("1. Среднее значение\n");
    printf("2. Сумма\n");
    printf("3. Количество\n");
    aggtype = _getch() - '0';
    if (aggtype != AGG_AVG && aggtype != AGG_SUM && aggtype != AGG_COUNT) {
        printf("Команда не распознана\n");
        _getch();
        return IS_SLOTUSED;
    }
    printf("---------%d\n", aggtype);
    group_arrchar_avg_int32(&(relslot[curslot].r), groupatt, aggatt, aggtype);
    return IS_SLOTUSED;
}

int gettuple_menu(relslottype* relslot, int curslot) {
    system("cls");
    relinfoprint(relslot[curslot].r);
    int Ntup;

    printf("Вставка строки:\n");
    printf("Введите количество вставляемых строк\n");
    while (1) {
        scanf("%d", &Ntup);
        if (Ntup < 1) {
            printf("Недопустимое значение\n");
        } else {
            break;
        }
    }

    printf("Ввод:");
    for (int i = 0; i < Ntup; i++) {
        gettuple(relslot[curslot].r);
    }

    return IS_SLOTUSED;
}
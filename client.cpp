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
                printf("�������� ����: (��� ������ ������� - \'e\')\n");
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
                    printf("������� �� ����������\n");
                    choice = 1;
                }
            } while (choice == 1);
            break;

        case IS_SLOTEMPTY:
            printf("��������� ����: %d\n", curslot);
            printf("��������� ��������:\n");
            printf("1. ������ ������� �� �����\n");
            printf("2. ���������� ������ �� ������� � ������ �������������� � ����\n");
            printf("3. �������� �������\n");
            printf("4. ����� �������\n");
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
                    printf("������� �� ����������\n");
                    choice = 1;
                    break;
                }

            } while (choice == 1);
            break;
        case IS_SLOTUSED:
            printf("��������� ����: %d\n", curslot);
            printf("��������� ��������: (��� ������ ������� - \'e\')\n");
            printf("1. ����� �������\n");
            printf("2. �������\n"); // � �������������� ���������� // � ��������� ��������
            printf("3. ���������� �� ������� � �������������� ����������\n");
            printf("4. ��������\n");
            printf("5. ����������� � ����� ����������� ���������: avg, sum, count\n");
            printf("6. ������� ������\n");
            printf("7. ������� �����\n");
            printf("8. ������ ������� � ����\n");
            printf("9. ����� ���������� � �������\n");
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
                    printf("������� �� ����������\n");
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
        printf("������������ ����.������ ������ ���� � ������� ��������, � max size: %d", size);
        return NULL;
    }

    if (quote) {
        c = getc(stdin);
        if (c == '"') { 
            return arrchar;
        } else {
            printf("������������ ����.������ ������ ���� � ������� ��������, � max size: %d", size);
        }
    } else {
        return arrchar;
    }
    return NULL;
}

rel* init_students(void) {
    int Natt = 4;
    relatt* att = (relatt*)malloc(sizeof(relatt) * Natt);

    att[0].name = (char*)malloc(strlen("id ��������")+1);
    strcpy(att[0].name, "id ��������");
    att[0].type = INT_32;
    att[0].size = 4;

    att[1].name = (char*)malloc(strlen("������ ���") + 1);
    strcpy(att[1].name, "������ ���");
    att[1].type = ARRCHAR;
    att[1].size = 25;

    att[2].name = (char*)malloc(strlen("���������") + 1);
    strcpy(att[2].name, "���������");
    att[2].type = ARRCHAR;
    att[2].size = 7;

    att[3].name = (char*)malloc(strlen("������") + 1);
    strcpy(att[3].name, "������");
    att[3].type = ARRCHAR;
    att[3].size = 7;

    rel* r = relinit(att, Natt);

    return r;
}

rel* init_grades(void) {
    int Natt = 6;
    relatt* att = (relatt*)malloc(sizeof(relatt) * Natt);

    att[0].name = (char*)malloc(strlen("id ������") + 1);
    strcpy(att[0].name, "id ������");
    att[0].type = INT_32;
    att[0].size = 4;

    att[1].name = (char*)malloc(strlen("id ��������") + 1);
    strcpy(att[1].name, "id ��������");
    att[1].type = INT_32;
    att[1].size = 4;

    att[2].name = (char*)malloc(strlen("����������") + 1);
    strcpy(att[2].name, "����������");
    att[2].type = ARRCHAR;
    att[2].size = 30;

    att[3].name = (char*)malloc(strlen("������") + 1);
    strcpy(att[3].name, "������");
    att[3].type = INT_32;
    att[3].size = 4;

    att[4].name = (char*)malloc(strlen("��� ������") + 1);
    strcpy(att[4].name, "��� ������");
    att[4].type = ARRCHAR;
    att[4].size = 10;

    att[5].name = (char*)malloc(strlen("�������") + 1);
    strcpy(att[5].name, "�������");
    att[5].type = INT_32;
    att[5].size = 4;

    rel* r = relinit(att, Natt);

    return r;
}

void slotprint(relslottype* relslot) {
    printf("�����:\n");
    for (int i = 0; i < SLOTCOUNT; i++) {
        printf("���� %d: %s\n", i, relslot[i].used == 1 ? "������������" : "������");
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

    printf("��������:\n");
    while (1) {
        printf("������� ���������� ��������, ������� ����� �������������:\n");
        scanf("%d", &Natt);
        if (Natt > relslot[curslot].r->Natt || Natt < 0) {
            printf("������������ ��������");
        } else {
            break;
        }
    }

    attarg = (int*)malloc(Natt * sizeof(int));

    printf("������� ������ ��������, ������� ����� �������������:\n");
    for (int i = 0; i < Natt; i++) {
        while (1) {
            scanf("%d", &attarg[i]);
            if (attarg[i] < 0 || attarg[i] > relslot[curslot].r->Natt - 1) {
                printf("������� \"%d\" �� ����������\n", attarg[i]);
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

    printf("�������: (��� ������ ������� - \'e\')\n");
    while (1) {
        printf("������� ����� �������, �� �������� ����� ����������� �������:\n");
        att = _getch() - '0';
        if (att < 0 || att > relslot[curslot].r->Natt - 1) {
            printf("������ ������� ���\n");
            _getch();
            return IS_SLOTUSED;
        } else {
            break;
        }
    } 
    if (relslot[curslot].r->att[att].type == INT_32) {

        printf("������� ������� ('=', '!', '>', '<'):\n");
        cond = _getch();
        if (cond != '=' && cond != '!' && cond != '>' && cond != '<') {
            printf("������ ������� ���\n");
            _getch();
            return IS_SLOTUSED;
        } 

        printf("������� �������� ��� ���������:\n");
        scanf("%d", &val_int32);

        whererel_int32(&(relslot[curslot].r), cond, att, val_int32);
    }
    else if (relslot[curslot].r->att[att].type == ARRCHAR) {

        printf("������� ������� ('=', '!'):\n");
        cond = _getch();
        if (cond != '=' && cond != '!') {
            printf("������ ������� ���\n");
            _getch();
            return IS_SLOTUSED;
        }

        printf("������� �������� ��� ���������:\n");
        do {
            val_arrchar = getarrchar(relslot[curslot].r->att[att].size);
        } while (val_arrchar == NULL);

        whererel_arrchar(&(relslot[curslot].r), cond, att, val_arrchar);
    } else {
        printf("�� ����� ������� ������ ������ �������");
    }

    return IS_SLOTUSED;
}

int createtemplate_menu(relslottype* relslot, int curslot) {
    system("cls");
    slotprint(relslot);

    int table;

    printf("�������� �������:\n");
    printf("������� ����� �����, �� �������� ����� ������ ������:\n");
    scanf("%d", &table);
    if (table > SLOTCOUNT || table < 0) {
        printf("������ ����� ���\n");
        _getch();
        return IS_SLOTEMPTY;
    } else if (relslot[table].used == 0) {
        printf("���� ���� ����\n");
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

    printf("����� �������:\n");
    printf("������� ����� �����, ������� ����� �������������:\n");
    scanf("%d", &table);
    if (table > SLOTCOUNT || table < 0) {
        printf("������ ����� ���\n");
        _getch();
        return IS_SLOTEMPTY;
    }
    else if (relslot[table].used == 0) {
        printf("���� ���� ����\n");
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

    printf("���������� ���� ������:\n");

    printf("������� ����� ������ �������\n");
    scanf("%d", &table1);
    if (table1 > SLOTCOUNT || table1 < 0) {
        printf("������ ����� ���\n");
        _getch();
        return IS_SLOTEMPTY;
    } else if (relslot[table1].used == 0) {
        printf("���� ���� ����\n");
        _getch();
        return IS_SLOTEMPTY;
    }

    printf("������� ����� ������� ������ �������, �� ������� ����� ����������� ����������: (��������� ���������� � ����)\n");
    scanf("%d", &att1);
    if ((att1 > relslot[table1].r->Natt - 1 || att1 < 0)) {
        printf("������ ������� ���\n");
        _getch();
        return IS_SLOTEMPTY;
    }

    printf("������� ����� ������ �������\n");
    scanf("%d", &table2);
    if (table2 > SLOTCOUNT || table2 < 0) {
        printf("������ ����� ���\n");
        _getch();
        return IS_SLOTEMPTY;
    } else if (relslot[table2].used == 0) {
        printf("���� ���� ����\n");
        _getch();
        return IS_SLOTEMPTY;
    }
    
    printf("������� ����� ������� ������ �������, �� ������� ����� ����������� ����������: (��������� ���������� � ����)\n");
    scanf("%d", &att2);
    if ((att2 > relslot[table1].r->Natt - 1 || att2 < 0)) {
        printf("������ ������� ���\n");
        _getch();
        return IS_SLOTEMPTY;
    }

    if (relslot[table1].r->att[att1].type == INT_32 && relslot[table2].r->att[att2].type == INT_32) {
        r = joinrel_int32(relslot[table1].r, att1, relslot[table2].r, att2);    
    } else if (relslot[table1].r->att[att1].type == ARRCHAR && relslot[table2].r->att[att2].type == ARRCHAR) {
        r = joinrel_arrchar(relslot[table1].r, att1, relslot[table2].r, att2);
    } else {
        printf("����� ��������� ������� ���� �� �������� � �������������� ����������, ���� �� �������� � ��������� ��������");
        return IS_SLOTEMPTY;
    }
    
    
    relslot[curslot].r = r;
    relslot[curslot].used = 1;
    return IS_SLOTUSED;
}

int writetable_menu(relslottype* relslot, int curslot) {
    system("cls");
    char choice;
        printf("�������, ��������� ��� ������:\n");
        printf("1. ��������:\n");
        printf("2. ������\n");
        choice = _getch();
        switch (choice) {
        case '1':
            writerel(relslot[curslot].r, "students");
            break;
        case '2':
            writerel(relslot[curslot].r, "grades");
            break;
        default:
            printf("������� �� ����������\n");
            _getch();
            return IS_SLOTUSED;
        }

    return IS_SLOTUSED;
}

int readtable_menu(relslottype* relslot, int curslot) {
    system("cls");
    char choice;
    rel* r = NULL;
    printf("�������, ��������� ��� ������:\n");
    printf("1. ��������:\n");
    printf("2. ������\n");
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
            printf("������� �� ����������\n");
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

    printf("������� ����� ������� �������, �� ������� ����� ����������� ����������:\n");
    scanf("%d", &att);
    if ((att > relslot[curslot].r->Natt - 1 || att < 0)) {
        printf("������ ������� ���\n");
        _getch();
        return IS_SLOTEMPTY;
    }

    printf("������� ��� ����������:\n");
    printf("1. �� �����������.\n");
    printf("2. �� ��������.\n");
    scanf("%d", &sorttype);
    if (sorttype != INCREASE && sorttype != DECREASE) {
        printf("������� �� ����������\n");
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

    printf("������� ����� �������, �� �������� ����� ��������� �����������\n");
    scanf("%d", &groupatt);
    if (groupatt < 0 || groupatt > relslot[curslot].r->Natt - 1) {
        printf("������� \"%d\" �� ����������\n", groupatt);
        _getch();
        return IS_SLOTUSED;
    } else if (relslot[curslot].r->att[groupatt].type != ARRCHAR) {
        printf("����������� ������ ���� �� ������� � ����� ������ ARRCHAR\n");
        _getch();
        return IS_SLOTUSED;
    }

    printf("������� ����� �������, �� �������� ����� ��������� �������������\n");
    scanf("%d", &aggatt);
    if (aggatt < 0 || aggatt > relslot[curslot].r->Natt - 1) {
        printf("������� \"%d\" �� ����������\n", aggatt);
        _getch();
        return IS_SLOTUSED;
    } else if (relslot[curslot].r->att[aggatt].type != INT_32) {
        printf("��������� ������ ���� �� ������� � ����� ������ INT_32\n");
        _getch();
        return IS_SLOTUSED;
    } 

    printf("�������� ����� �������������:\n");
    printf("1. ������� ��������\n");
    printf("2. �����\n");
    printf("3. ����������\n");
    aggtype = _getch() - '0';
    if (aggtype != AGG_AVG && aggtype != AGG_SUM && aggtype != AGG_COUNT) {
        printf("������� �� ����������\n");
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

    printf("������� ������:\n");
    printf("������� ���������� ����������� �����\n");
    while (1) {
        scanf("%d", &Ntup);
        if (Ntup < 1) {
            printf("������������ ��������\n");
        } else {
            break;
        }
    }

    printf("����:");
    for (int i = 0; i < Ntup; i++) {
        gettuple(relslot[curslot].r);
    }

    return IS_SLOTUSED;
}
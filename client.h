#pragma once
#include "relations.h"

typedef struct relslottype {
    rel* r;
    int used; // true or false
} relslottype;

#define SLOTCOUNT   8

#define IS_EXIT         0
#define IS_SLOTCHOICE   1
#define IS_SLOTEMPTY    2
#define IS_SLOTUSED     3

// ����:
int projection_menu(relslottype* relslot, int curslot);
int where_menu(relslottype* relslot, int curslot);
int createtemplate_menu(relslottype* relslot, int curslot);
int cpytable_menu(relslottype* relslot, int curslot);
int join_menu(relslottype* relslot, int curslot);
int writetable_menu(relslottype* relslot, int curslot); 
int readtable_menu(relslottype* relslot, int curslot);
int sort_menu(relslottype* relslot, int curslot);
int group_menu(relslottype* relslot, int curslot);
int gettuple_menu(relslottype* relslot, int curslot);

// ������������� ������ �� ���������� � ��������
rel* init_students(void);
rel* init_grades(void);

// utils
char* getarrchar(int size);
void slotprint(relslottype* relslot); // ����� ���� ������
int freeslot(relslottype* relslot, int curslot); // ������������ ����� 

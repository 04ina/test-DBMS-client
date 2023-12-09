#pragma once
#define INT_16          0
#define INT_32          1
#define INT_64          2
#define FLOAT_32        3 
#define DOUBLE_64       4 
#define ARRCHAR         5
#define PRIMARYKEY      6

#define AGG_AVG         1
#define AGG_SUM         2
#define AGG_COUNT       3

#define INCREASE        1
#define DECREASE        2

#define max(a, b)  (((a) > (b)) ? (a) : (b)) 
#define min(a, b)  (((a) < (b)) ? (a) : (b)) 

typedef struct tupnode {
    void* data;
    struct tupnode* next;
} tupnode;

typedef struct SLlist {
    tupnode* head;
    tupnode* end;
} SLlist;

// primary key
typedef struct primarykey {
    int id;
    int last; // for increment
} primarykey;

// relation attributes
typedef struct relatt {
    char* name;
    int type;
    int size;
} relatt;

// relation tuples
typedef SLlist reltuplist;

// relation
typedef struct rel {
    relatt* att;
    reltuplist tup;
    int sizetup;
    int Natt;
    int Ntup;
} rel;


// Выборка по столбцу с целочисленными значениями 
// аргумент cond определяет, какой оператор сравнения будет использоваться. Доступны операторы: < > = !(!=)
void whererel_int32(rel** rarg, char cond, int attnum, int val);

// Выборка по столбцу с массивами символов 
// аргумент cond определяет, какой оператор сравнения будет использоваться. Доступны операторы: = !(!=)
void whererel_arrchar(rel** rarg, char cond, int attnum, const char* val);

// Сортировка по столбцу с целочисленными значениями
int sortrel_int32(rel* r, int attnum, int sorttype);

// Соединение таблиц по столбцу с массивами символов
rel* joinrel_int32(rel* r1, int attnum1, rel* r2, int attnum2);

// Соединение таблиц по столбцу с целочисленными значениями
rel* joinrel_arrchar(rel* r1, int attnum1, rel* r2, int attnum2);

// Проекция
rel* projectionrel(rel** rarg, int* attarg, int Natt);

// Группировка с тремя анрегатными функциями: avg, sum, count
rel* group_arrchar_avg_int32(rel** rarg, int groupatt, int aggatt, int aggtype);

// Вставка строки
void gettuple(rel* r);

// Удаление таблицы
void delrel(rel* r);

// Копия таблицы без данных 
rel* relcpy_nodata(rel* r);

// Копия таблицы c данными
rel* relcpy(rel* r);

// Вывод таблицы в консоль
void relprint(rel* r);

// Чтение таблицы из файла
void readrel(rel* r, const char* name);

// Запись таблицы в файл
void writerel(rel* r, const char* name);

// Вывод информации о таблице
void relinfoprint(rel* r);

// Инициализация таблицы 
rel* relinit(relatt* att, int Natt);

// utils
void print_strip(rel* r, int* maxsize);
void tlist_pushback(rel* r, void* data, int size);
void merge_int32(int* valarr, tupnode** tuparr, int l, int r, int sorttype);
const char* atttypeget(int type);
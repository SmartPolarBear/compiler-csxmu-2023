#ifndef TYPES_H
#define TYPES_H

#include <stdbool.h>

#define MAXLEN 256
#define CODE_BEGIN 100
#define SYMBOL_BEGIN 1000
#define CONSTANT_BEGIN 3000

typedef enum type
{
    INT,
    FLOAT,
    CHAR,
    BOOL
} type;

typedef enum op
{
    addi,
    subi,
    muli,
    divi,
    invi,
    addf,
    subf,
    mulf,
    divf,
    invf,
    movi,
    movf,
    movc,
    movb,
    itof,
    itoc,
    itob,
    ftoi,
    ftoc,
    ftob,
    ctoi,
    ctof,
    ctob,
    btoi,
    btof,
    btoc,
    and,
    or,
    not,
    eq,
    ne,
    gt,
    ge,
    lt,
    le,
    jmp,
    jeq,
    jne,
    jgt,
    jge,
    jlt,
    jle
} op;

typedef struct symbol
{
    char name[MAXLEN];
    type type;
    int addr;
    int width;
    struct symbol *next;
} symbol;

typedef struct symbol_list
{
    symbol *head;
    struct symbol_list *prev;
    int begin;
    int end;
} symbol_list;

typedef union constant_val
{
    char ch;
    int num;
    double real;
    bool boolean;
} constant_val;

typedef struct constant
{
    constant_val val;
    type type;
    int addr;
    int width;
    struct constant *next;
} constant;

typedef struct constant_list
{
    constant *head;
    int begin;
    int end;
} constant_list;

typedef struct goto_list
{
    int addr;
    struct goto_list *next;
} goto_list;

typedef struct yystack
{
    char name[MAXLEN];
    constant_val val;
    type type;
    int addr;
    goto_list *t_list;
    goto_list *f_list;
    goto_list *n_list;
} yystack;

typedef struct quadruple
{
    int op;
    int arg1;
    int arg2;
    int arg3;
    char code[4 * MAXLEN];
} quadruple;

typedef struct quadtable
{
    quadruple *buf;
    int size;
    int bufsize;
} quadtable;

#define YYSTYPE yystack

symbol *new_symbol(symbol_list *list, char *name, type type);
int new_temp(symbol_list *list, type type);
symbol *find_symbol(symbol_list *list, char *name);
symbol *find_local_symbol(symbol_list *list, char *name);
symbol_list *new_symbol_list(symbol_list *prev, int addr);
void print_symbol_list(symbol_list *list);
void delete_symbol_list(symbol_list *list);

constant *new_constant(constant_list *list, constant_val val, type type);
constant *find_constant(constant_list *list, constant_val val, type type);
constant_list *new_constant_list(int addr);
void print_constant_list(constant_list *list);
void delete_constant_list(constant_list *list);

goto_list *new_goto_list(int addr);
goto_list *merge_goto_list(goto_list *list1, goto_list *list2);
void delete_goto_list(goto_list *list);

void gen(quadtable *table, symbol_list *slist, constant_list *clist, int op, int arg1, int arg2, int arg3);
int widen(quadtable *table, symbol_list *slist, constant_list *clist, int addr, type type1, type type2);
void backpatch(quadtable *table, goto_list *list, int addr);
quadtable *new_quadtable();
void print_quadtable(quadtable *table);
void delete_quadtable(quadtable* table);

void save_constant_list(FILE* fp, constant_list* list);
void save_quadtable(FILE* fp, quadtable* table);

#endif
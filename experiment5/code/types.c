#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "types.h"

int temp_count = 1;

int type_width[4] = { 4, 8, 1, 1 };
char* type_name[4] = { "int", "float", "char", "bool" };
char* op_name[42] = {
    "addi", "subi", "muli", "divi", "invi",
    "addf", "subf", "mulf", "divf", "invf",
    "movi", "movf", "movc", "movb",
    "itof", "itoc", "itob",
    "ftoi", "ftoc", "ftob",
    "ctoi", "ctof", "ctob",
    "btoi", "btof", "btoc",
    "and", "or", "not", "eq", "ne", "gt", "ge", "lt", "le",
    "jmp", "jeq", "jne", "jgt", "jge", "jlt", "jle" };
int conv[4][4] = {
    {-1, itof, itoc, itob},
    {ftoi, -1, ftoc, ftob},
    {ctoi, ctof, -1, ctob},
    {btoi, btof, btoc, -1} };

symbol* new_symbol(symbol_list* list, char* name, type type)
{
    symbol* id = malloc(sizeof(symbol));
    strcpy(id->name, name);
    id->type = type;
    id->width = type_width[type];
    id->addr = list->end;
    list->end += id->width;
    id->next = list->head;
    list->head = id;
    return id;
}

int new_temp(symbol_list* list, type type)
{
    symbol* id = malloc(sizeof(symbol));
    sprintf(id->name, "_t%d", temp_count++);
    id->type = type;
    id->width = type_width[type];
    id->addr = list->end;
    list->end += id->width;
    id->next = list->head;
    list->head = id;
    return id->addr;
}

symbol* find_symbol(symbol_list* list, char* name)
{
    symbol* id;
    symbol_list* list2 = list;
    while (list2 != NULL)
    {
        id = find_local_symbol(list2, name);
        if (id != NULL)
            return id;
        list2 = list2->prev;
    }
    return NULL;
}

symbol* find_symbol_by_addr(symbol_list* list, int addr)
{
    symbol* id;
    symbol_list* list2 = list;
    while (list2 != NULL)
    {
        symbol* id2 = list2->head;
        while (id2 != NULL)
        {
            if (id2->addr == addr)
                return id2;
            id2 = id2->next;
        }
        list2 = list2->prev;
    }
    return NULL;
}

symbol* find_local_symbol(symbol_list* list, char* name)
{
    symbol* id = list->head;
    while (id != NULL)
    {
        if (strcmp(id->name, name) == 0)
            return id;
        id = id->next;
    }
    return NULL;
}

symbol_list* new_symbol_list(symbol_list* prev, int addr)
{
    symbol_list* list = malloc(sizeof(symbol_list));
    list->head = NULL;
    list->prev = prev;
    list->begin = addr;
    list->end = addr;
    return list;
}

void print_symbol_list(symbol_list* list)
{
    symbol* id = list->head;
    printf("\n--------- symbol list ---------\n");
    printf("name\ttype\taddr\twidth\n");
    printf("-------------------------------\n");
    while (id != NULL)
    {
        printf("%s\t%s\t%d\t%d\n", id->name, type_name[id->type], id->addr, id->width);
        id = id->next;
    }
    printf("-------------------------------\n\n");
}

void delete_symbol_list(symbol_list* list)
{
    if (list == NULL)
        return;
    symbol* id = list->head, * id2;
    while (id != NULL)
    {
        id2 = id->next;
        free(id);
        id = id2;
    }
    free(list);
}

constant* new_constant(constant_list* list, constant_val val, type type)
{
    constant* c = find_constant(list, val, type);
    if (c == NULL)
    {
        c = malloc(sizeof(constant));
        c->val = val;
        c->type = type;
        c->width = type_width[type];
        c->addr = list->end;
        list->end += c->width;
        c->next = list->head;
        list->head = c;
    }
    return c;
}

constant* find_constant(constant_list* list, constant_val val, type type)
{
    constant* c = list->head;
    while (c != NULL)
    {
        if (c->type == type && memcmp(&c->val, &val, c->width) == 0)
            return c;
        c = c->next;
    }
    return NULL;
}

constant* find_constant_by_addr(constant_list* list, int addr)
{
    constant* c = list->head;
    while (c != NULL)
    {
        if (c->addr == addr)
            return c;
        c = c->next;
    }
    return NULL;
}

constant_list* new_constant_list(int addr)
{
    constant_list* list = malloc(sizeof(constant_list));
    list->head = NULL;
    list->begin = addr;
    list->end = addr;
    return list;
}

void print_constant_list(constant_list* list)
{
    constant* c = list->head;
    printf("\n-------- constant list --------\n");
    printf("val\ttype\taddr\twidth\n");
    printf("-------------------------------\n");
    while (c != NULL)
    {
        switch (c->type)
        {
        case INT:
            printf("%d\t", c->val.num);
            break;
        case FLOAT:
            printf("%.9g\t", c->val.real);
            break;
        case CHAR:
            printf("'%c'\t", c->val.ch);
            break;
        case BOOL:
            printf("%s\t", c->val.boolean ? "true" : "false");
            break;
        }
        printf("%s\t%d\t%d\n", type_name[c->type], c->addr, c->width);
        c = c->next;
    }
    printf("-------------------------------\n\n");
}

void delete_constant_list(constant_list* list)
{
    if (list == NULL)
        return;
    constant* c = list->head, * c2;
    while (c != NULL)
    {
        c2 = c->next;
        free(c);
        c = c2;
    }
    free(list);
}

goto_list* new_goto_list(int addr)
{
    goto_list* list = malloc(sizeof(goto_list));
    list->addr = addr;
    list->next = NULL;
    return list;
}

goto_list* merge_goto_list(goto_list* list1, goto_list* list2)
{
    goto_list* list = list1;
    if (list1 == NULL && list2 == NULL)
        return NULL;
    if (list1 == NULL)
        return list2;
    if (list2 == NULL)
        return list1;
    while (list1->next != NULL)
        list1 = list1->next;
    list1->next = list2;
    return list;
}

void delete_goto_list(goto_list* list)
{
    goto_list* list2;
    while (list != NULL)
    {
        list2 = list->next;
        free(list);
        list = list2;
    }
}

void arg_name(char* code, symbol_list* slist, constant_list* clist, int addr)
{
    if (addr == -1)
    {
        code[0] = '\0';
        return;
    }
    if (addr >= CONSTANT_BEGIN)
    {
        constant* c = find_constant_by_addr(clist, addr);
        switch (c->type)
        {
        case INT:
            sprintf(code, "%d", c->val.num);
            break;
        case FLOAT:
            sprintf(code, "%.9g", c->val.real);
            break;
        case CHAR:
            sprintf(code, "'%c'", c->val.ch);
            break;
        case BOOL:
            sprintf(code, "%s", c->val.boolean ? "true" : "false");
            break;
        }
    }
    else if (addr >= SYMBOL_BEGIN)
    {
        symbol* id = find_symbol_by_addr(slist, addr);
        sprintf(code, "%s", id->name);
    }
    else
        sprintf(code, "%d", addr);
}

void gen(quadtable* table, symbol_list* slist, constant_list* clist, int op, int arg1, int arg2, int arg3)
{
    if (table->size == table->bufsize)
    {
        table->bufsize += MAXLEN;
        table->buf = realloc(table->buf, table->bufsize);
    }
    quadruple* t = table->buf + table->size;

    t->op = op;
    t->arg1 = arg1;
    t->arg2 = arg2;
    t->arg2 = arg2;
    t->arg3 = arg3;

    int addr = table->size + CODE_BEGIN;
    char code1[MAXLEN];
    char code2[MAXLEN];
    char code3[MAXLEN];
    arg_name(code1, slist, clist, t->arg1);
    arg_name(code2, slist, clist, t->arg2);
    arg_name(code3, slist, clist, t->arg3);

    switch (t->op)
    {
    case movi:
    case movf:
    case movc:
    case movb:
        sprintf(t->code, "%d:\t%s = %s\n", addr, code3, code1);
        break;
    case addi:
    case addf:
        sprintf(t->code, "%d:\t%s = %s + %s\n", addr, code3, code1, code2);
        break;
    case subi:
    case subf:
        sprintf(t->code, "%d:\t%s = %s - %s\n", addr, code3, code1, code2);
        break;
    case muli:
    case mulf:
        sprintf(t->code, "%d:\t%s = %s * %s\n", addr, code3, code1, code2);
        break;
    case divi:
    case divf:
        sprintf(t->code, "%d:\t%s = %s / %s\n", addr, code3, code1, code2);
        break;
    caseand :
        sprintf(t->code, "%d:\t%s = %s && %s\n", addr, code3, code1, code2);
        break;
    case or :
        sprintf(t->code, "%d:\t%s = %s || %s\n", addr, code3, code1, code2);
        break;
    case eq:
        sprintf(t->code, "%d:\t%s = %s == %s\n", addr, code3, code1, code2);
        break;
    case ne:
        sprintf(t->code, "%d:\t%s = %s != %s\n", addr, code3, code1, code2);
        break;
    case gt:
        sprintf(t->code, "%d:\t%s = %s > %s\n", addr, code3, code1, code2);
        break;
    case ge:
        sprintf(t->code, "%d:\t%s = %s >= %s\n", addr, code3, code1, code2);
        break;
    case lt:
        sprintf(t->code, "%d:\t%s = %s < %s\n", addr, code3, code1, code2);
        break;
    case le:
        sprintf(t->code, "%d:\t%s = %s <= %s\n", addr, code3, code1, code2);
        break;
    case invi:
    case invf:
        sprintf(t->code, "%d:\t%s = -%s\n", addr, code3, code1);
        break;
    case not:
        sprintf(t->code, "%d:\t%s = !%s\n", addr, code3, code1);
        break;
    case ftoi:
    case ctoi:
    case btoi:
        sprintf(t->code, "%d:\t%s = (int)%s\n", addr, code3, code1);
        break;
    case itof:
    case ctof:
    case btof:
        sprintf(t->code, "%d:\t%s = (float)%s\n", addr, code3, code1);
        break;
    case itoc:
    case ftoc:
    case btoc:
        sprintf(t->code, "%d:\t%s = (char)%s\n", addr, code3, code1);
        break;
    case itob:
    case ftob:
    case ctob:
        sprintf(t->code, "%d:\t%s = (bool)%s\n", addr, code3, code1);
        break;
    case jmp:
        sprintf(t->code, "%d:\tgoto %s\n", addr, code3);
        break;
    case jeq:
        sprintf(t->code, "%d:\tif %s == %s goto %s\n", addr, code1, code2, code3);
        break;
    case jne:
        sprintf(t->code, "%d:\tif %s != %s goto %s\n", addr, code1, code2, code3);
        break;
    case jgt:
        sprintf(t->code, "%d:\tif %s > %s goto %s\n", addr, code1, code2, code3);
        break;
    case jge:
        sprintf(t->code, "%d:\tif %s >= %s goto %s\n", addr, code1, code2, code3);
        break;
    case jlt:
        sprintf(t->code, "%d:\tif %s < %s goto %s\n", addr, code1, code2, code3);
        break;
    case jle:
        sprintf(t->code, "%d:\tif %s <= %s goto %s\n", addr, code1, code2, code3);
        break;
    }
    table->size++;
}

int widen(quadtable* table, symbol_list* slist, constant_list* clist, int addr, type type1, type type2)
{
    int op = conv[type1][type2];
    if (op != -1)
    {
        int arg3 = new_temp(slist, type2);
        gen(table, slist, clist, op, addr, -1, arg3);
        return arg3;
    }
    return addr;
}

void backpatch(quadtable* table, goto_list* list, int addr)
{
    while (list != NULL)
    {
        quadruple* t = table->buf + list->addr - CODE_BEGIN;
        t->arg3 = addr;
        sprintf(t->code + strlen(t->code) - 1, "%d\n", addr);
        list = list->next;
    }
}

quadtable* new_quadtable()
{
    quadtable* table = malloc(sizeof(quadtable));
    table->buf = malloc(sizeof(quadruple) * MAXLEN);
    table->size = 0;
    table->bufsize = MAXLEN;
    return table;
}

void print_quadtable(quadtable* table)
{
    printf("\n-------------- quadtable --------------\n");
    printf("addr\top\targ1\targ2\tresult\n");
    printf("---------------------------------------\n");
    for (int i = 0; i < table->size; i++)
        printf("%d\t%s\t%d\t%d\t%d\n", i + CODE_BEGIN, op_name[table->buf[i].op], table->buf[i].arg1, table->buf[i].arg2, table->buf[i].arg3);
    printf("---------------------------------------\n\n");
    for (int i = 0; i < table->size; i++)
        printf("%s", table->buf[i].code);
}

void delete_quadtable(quadtable* table)
{
    free(table->buf);
    free(table);
}


void save_constant_list(FILE* fp, constant_list* list)
{
    constant* c = list->head;
    fprintf(fp, "\n-------- constant list --------\n");
    fprintf(fp, "val\ttype\taddr\twidth\n");
    fprintf(fp, "-------------------------------\n");
    while (c != NULL)
    {
        switch (c->type)
        {
        case INT:
            fprintf(fp,"%d\t", c->val.num);
            break;
        case FLOAT:
            fprintf(fp,"%.9g\t", c->val.real);
            break;
        case CHAR:
            fprintf(fp,"'%c'\t", c->val.ch);
            break;
        case BOOL:
            fprintf(fp,"%s\t", c->val.boolean ? "true" : "false");
            break;
        }
        fprintf(fp,"%s\t%d\t%d\n", type_name[c->type], c->addr, c->width);
        c = c->next;
    }
    fprintf(fp,"-------------------------------\n\n");
}

void save_quadtable(FILE* fp, quadtable* table)
{
    fprintf(fp,"\n-------------- quadtable --------------\n");
    fprintf(fp,"addr\top\targ1\targ2\tresult\n");
    fprintf(fp,"---------------------------------------\n");
    for (int i = 0; i < table->size; i++)
        fprintf(fp,"%d\t%s\t%d\t%d\t%d\n", i + CODE_BEGIN, op_name[table->buf[i].op], table->buf[i].arg1, table->buf[i].arg2, table->buf[i].arg3);
    fprintf(fp,"---------------------------------------\n\n");
    for (int i = 0; i < table->size; i++)
        fprintf(fp,"%s", table->buf[i].code);
}
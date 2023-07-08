%{
#include <stdio.h>
#include <stdlib.h>

#include "lex.yy.c"

#pragma GCC diagnostic ignored "-Wformat-truncation" 

int _mov[4] = {movi, movf, movc, movb};
int _add[2] = {addi, addf};
int _sub[2] = {subi, subf};
int _mul[2] = {muli, mulf};
int _div[2] = {divi, divf};
int _inv[2] = {invi, invf};
int max[4][4] = {
    {INT, FLOAT, INT, INT},
    {FLOAT, FLOAT, FLOAT, FLOAT},
    {INT, FLOAT, CHAR, INT},
    {INT, FLOAT, INT, BOOL}
};

quadtable *table = NULL;
symbol_list *slist = NULL;
constant_list *clist = NULL;
goto_list *break_lists[MAXLEN];
int tos = 0;

int error = 0;

void yyerror(char *msg)
{
    error++;
    fprintf(stderr, "\033[1mline %d: %s\033[0m\n", line, msg);
}
%}

%token IF ELSE DO WHILE BREAK
%token BASIC ID CONST 

%left OR
%left AND
%left EQ NE
%left LT LE GT GE
%left '+' '-'
%left '*' '/'
%right INV NOT

%%

program : block
block   : '{' begin decls stmts end '}'
begin   :
        {
            if (slist == NULL)
                slist = new_symbol_list(NULL, SYMBOL_BEGIN);
            else
                slist = new_symbol_list(slist, slist->end);
        }
end     :
        {
            symbol_list *slist2 = slist;
            if (error == 0)
                print_symbol_list(slist2);
            slist = slist2->prev;
            delete_symbol_list(slist2);
        }
decls   : decls decl
        |
decl    : BASIC ID ';'
        {
            if (find_local_symbol(slist, $2.name) == NULL)
                new_symbol(slist, $2.name, $1.type);
            else
            {
                char msg[MAXLEN];
                snprintf(msg,MAXLEN,"duplicate declaration of symbol \"%s\"", $2.name);
                yyerror(msg);
            }
        }
stmts   : stmts stmt M
        {   backpatch(table, $2.n_list, $3.addr);  }
        |
M       :
        {   $$.addr = table->size + CODE_BEGIN;  }
N       :
        {
            $$.n_list = new_goto_list(table->size + CODE_BEGIN);
            gen(table, slist, clist, jmp, -1, -1, -1);
        }
Q       :
        {   break_lists[tos++] = NULL;  }
stmt    : ID '=' expr ';'
        {
            $$.n_list = NULL;
            symbol *id = find_symbol(slist, $1.name);
            if (id == NULL)
            {
                char msg[MAXLEN];
                snprintf(msg,MAXLEN, "undeclared symbol \"%s\"", $1.name);
                yyerror(msg);
                id = new_symbol(slist, $1.name, INT);
            }
            int arg1 = widen(table, slist, clist, $3.addr, $3.type, id->type);
            int arg3 = id->addr;
            gen(table, slist, clist, _mov[id->type], arg1, -1, arg3);
        }
        | IF '(' bool ')' M stmt
        {
            backpatch(table, $3.t_list, $5.addr);
            $$.n_list = merge_goto_list($3.f_list, $6.n_list);
        }
        | IF '(' bool ')' M stmt N ELSE M stmt
        {
            backpatch(table, $3.t_list, $5.addr);
            backpatch(table, $3.f_list, $9.addr);
            $$.n_list = merge_goto_list(merge_goto_list($6.n_list, $7.n_list), $10.n_list);
        }
        | WHILE Q M '(' bool ')' M stmt
        {
            backpatch(table, $8.n_list, $3.addr);
            backpatch(table, $5.t_list, $7.addr);
            $$.n_list = merge_goto_list($5.f_list, break_lists[--tos]);
            gen(table, slist, clist, jmp, -1, -1, $3.addr);
        }
        | DO Q M stmt WHILE M '(' bool ')' ';'
        {
            backpatch(table, $8.t_list, $3.addr);
            backpatch(table, $4.n_list, $6.addr);
            $$.n_list = merge_goto_list($8.f_list, break_lists[--tos]);
        }
        | BREAK ';'
        {
            $$.n_list = NULL;
            if (tos == 0)
                yyerror("\"break\" statement doesn't match any loop");
            else
            {
                break_lists[tos - 1] = merge_goto_list(break_lists[tos - 1], new_goto_list(table->size + CODE_BEGIN));
                gen(table, slist, clist, jmp, -1, -1, -1);
            }
        }
        | block
        {   $$.n_list = NULL;  }
        | error ';'
bool    : bool OR M bool
        {
            backpatch(table, $1.f_list, $3.addr);
            $$.t_list = merge_goto_list($1.t_list, $4.t_list);
            $$.f_list = $4.f_list;
        }
        | bool AND M bool
        {
            backpatch(table, $1.t_list, $3.addr);
            $$.t_list = $4.t_list;
            $$.f_list = merge_goto_list($1.f_list, $4.f_list);
        }
        | '!' bool %prec NOT
        {
            $$.t_list = $2.f_list;
            $$.f_list = $2.t_list;
        }
        | '(' bool ')'
        {
            $$.t_list = $2.t_list;
            $$.f_list = $2.f_list;
        }
        | expr EQ expr
        {
            int arg1 = widen(table, slist, clist, $1.addr, $1.type, max[$1.type][$3.type]);
            int arg2 = widen(table, slist, clist, $3.addr, $3.type, max[$1.type][$3.type]);
            $$.t_list = new_goto_list(table->size + CODE_BEGIN);
            $$.f_list = new_goto_list(table->size + CODE_BEGIN + 1);
            gen(table, slist, clist, jeq, arg1, arg2, -1);
            gen(table, slist, clist ,jmp, -1, -1, -1);
        }
        | expr NE expr
        {
            int arg1 = widen(table, slist, clist, $1.addr, $1.type, max[$1.type][$3.type]);
            int arg2 = widen(table, slist, clist, $3.addr, $3.type, max[$1.type][$3.type]);
            $$.t_list = new_goto_list(table->size + CODE_BEGIN);
            $$.f_list = new_goto_list(table->size + CODE_BEGIN + 1);
            gen(table, slist, clist, jne, arg1, arg2, -1);
            gen(table, slist, clist ,jmp, -1, -1, -1);
        }
        | expr LT expr
        {
            int arg1 = widen(table, slist, clist, $1.addr, $1.type, max[$1.type][$3.type]);
            int arg2 = widen(table, slist, clist, $3.addr, $3.type, max[$1.type][$3.type]);
            $$.t_list = new_goto_list(table->size + CODE_BEGIN);
            $$.f_list = new_goto_list(table->size + CODE_BEGIN + 1);
            gen(table, slist, clist, jlt, arg1, arg2, -1);
            gen(table, slist, clist ,jmp, -1, -1, -1);
        }
        | expr LE expr
        {
            int arg1 = widen(table, slist, clist, $1.addr, $1.type, max[$1.type][$3.type]);
            int arg2 = widen(table, slist, clist, $3.addr, $3.type, max[$1.type][$3.type]);
            $$.t_list = new_goto_list(table->size + CODE_BEGIN);
            $$.f_list = new_goto_list(table->size + CODE_BEGIN + 1);
            gen(table, slist, clist, jle, arg1, arg2, -1);
            gen(table, slist, clist ,jmp, -1, -1, -1);
        }
        | expr GT expr
        {
            int arg1 = widen(table, slist, clist, $1.addr, $1.type, max[$1.type][$3.type]);
            int arg2 = widen(table, slist, clist, $3.addr, $3.type, max[$1.type][$3.type]);
            $$.t_list = new_goto_list(table->size + CODE_BEGIN);
            $$.f_list = new_goto_list(table->size + CODE_BEGIN + 1);
            gen(table, slist, clist, jgt, arg1, arg2, -1);
            gen(table, slist, clist ,jmp, -1, -1, -1);
        }
        | expr GE expr
        {
            int arg1 = widen(table, slist, clist, $1.addr, $1.type, max[$1.type][$3.type]);
            int arg2 = widen(table, slist, clist, $3.addr, $3.type, max[$1.type][$3.type]);
            $$.t_list = new_goto_list(table->size + CODE_BEGIN);
            $$.f_list = new_goto_list(table->size + CODE_BEGIN + 1);
            gen(table, slist, clist, jge, arg1, arg2, -1);
            gen(table, slist, clist ,jmp, -1, -1, -1);
        }
        | expr
        {
            int arg1 = widen(table, slist, clist, $1.addr, $1.type, BOOL);
            int arg2 = new_constant(clist, (constant_val)true, BOOL)->addr;
            $$.t_list = new_goto_list(table->size + CODE_BEGIN);
            $$.f_list = new_goto_list(table->size + CODE_BEGIN + 1);
            gen(table, slist, clist, jeq, arg1, arg2, -1);
            gen(table, slist, clist ,jmp, -1, -1, -1);
        }
expr    : expr OR expr
        {
            $$.type = BOOL;
            int arg1 = widen(table, slist, clist, $1.addr, $1.type, $$.type);
            int arg2 = widen(table, slist, clist, $3.addr, $3.type, $$.type);
            int arg3 = new_temp(slist, $$.type);
            gen(table, slist, clist, or, arg1, arg2, arg3);
            $$.addr = arg3;
        }
        | expr AND expr
        {
            $$.type = BOOL;
            int arg1 = widen(table, slist, clist, $1.addr, $1.type, $$.type);
            int arg2 = widen(table, slist, clist, $3.addr, $3.type, $$.type);
            int arg3 = new_temp(slist, $$.type);
            gen(table, slist, clist, and, arg1, arg2, arg3);
            $$.addr = arg3;
        }
        | expr EQ expr
        {
            $$.type = BOOL;
            int arg1 = widen(table, slist, clist, $1.addr, $1.type, max[$1.type][$3.type]);
            int arg2 = widen(table, slist, clist, $3.addr, $3.type, max[$1.type][$3.type]);
            int arg3 = new_temp(slist, $$.type);
            gen(table, slist, clist, eq, arg1, arg2, arg3);
            $$.addr = arg3;
        }
        | expr NE expr
        {
            $$.type = BOOL;
            int arg1 = widen(table, slist, clist, $1.addr, $1.type, max[$1.type][$3.type]);
            int arg2 = widen(table, slist, clist, $3.addr, $3.type, max[$1.type][$3.type]);
            int arg3 = new_temp(slist, $$.type);
            gen(table, slist, clist, ne, arg1, arg2, arg3);
            $$.addr = arg3;
        }
        | expr LT expr
        {
            $$.type = BOOL;
            int arg1 = widen(table, slist, clist, $1.addr, $1.type, max[$1.type][$3.type]);
            int arg2 = widen(table, slist, clist, $3.addr, $3.type, max[$1.type][$3.type]);
            int arg3 = new_temp(slist, $$.type);
            gen(table, slist, clist, lt, arg1, arg2, arg3);
            $$.addr = arg3;
        }
        | expr LE expr
        {
            $$.type = BOOL;
            int arg1 = widen(table, slist, clist, $1.addr, $1.type, max[$1.type][$3.type]);
            int arg2 = widen(table, slist, clist, $3.addr, $3.type, max[$1.type][$3.type]);
            int arg3 = new_temp(slist, $$.type);
            gen(table, slist, clist, le, arg1, arg2, arg3);
            $$.addr = arg3;
        }
        | expr GT expr
        {
            $$.type = BOOL;
            int arg1 = widen(table, slist, clist, $1.addr, $1.type, max[$1.type][$3.type]);
            int arg2 = widen(table, slist, clist, $3.addr, $3.type, max[$1.type][$3.type]);
            int arg3 = new_temp(slist, $$.type);
            gen(table, slist, clist, gt, arg1, arg2, arg3);
            $$.addr = arg3;
        }
        | expr GE expr
        {
            $$.type = BOOL;
            int arg1 = widen(table, slist, clist, $1.addr, $1.type, max[$1.type][$3.type]);
            int arg2 = widen(table, slist, clist, $3.addr, $3.type, max[$1.type][$3.type]);
            int arg3 = new_temp(slist, $$.type);
            gen(table, slist, clist, ge, arg1, arg2, arg3);
            $$.addr = arg3;
        }
        | expr '+' expr
        {
            $$.type = max[$1.type][$3.type];
            int arg1 = widen(table, slist, clist, $1.addr, $1.type, $$.type);
            int arg2 = widen(table, slist, clist, $3.addr, $3.type, $$.type);
            int arg3 = new_temp(slist, $$.type);
            gen(table, slist, clist, _add[$$.type], arg1, arg2, arg3);
            $$.addr = arg3;
        }
        | expr '-' expr
        {
            $$.type = max[$1.type][$3.type];
            int arg1 = widen(table, slist, clist, $1.addr, $1.type, $$.type);
            int arg2 = widen(table, slist, clist, $3.addr, $3.type, $$.type);
            int arg3 = new_temp(slist, $$.type);
            gen(table, slist, clist, _sub[$$.type], arg1, arg2, arg3);
            $$.addr = arg3;
        }
        | expr '*' expr
        {
            $$.type = max[$1.type][$3.type];
            int arg1 = widen(table, slist, clist, $1.addr, $1.type, $$.type);
            int arg2 = widen(table, slist, clist, $3.addr, $3.type, $$.type);
            int arg3 = new_temp(slist, $$.type);
            gen(table, slist, clist, _mul[$$.type], arg1, arg2, arg3);
            $$.addr = arg3;
        }
        | expr '/' expr
        {
            $$.type = max[$1.type][$3.type];
            int arg1 = widen(table, slist, clist, $1.addr, $1.type, $$.type);
            int arg2 = widen(table, slist, clist, $3.addr, $3.type, $$.type);
            int arg3 = new_temp(slist, $$.type);
            gen(table, slist, clist, _div[$$.type], arg1, arg2, arg3);
            $$.addr = arg3;
        }
        | '!' expr %prec NOT
        {
            $$.type = BOOL;
            int arg1 = widen(table, slist, clist, $2.addr, $2.type, $$.type);
            int arg3 = new_temp(slist, $$.type);
            gen(table, slist, clist, not, arg1, -1, arg3);
            $$.addr = arg3;
        }
        | '-' expr %prec INV
        {
            $$.type = max[$2.type][INT];
            int arg1 = widen(table, slist, clist, $2.addr, $2.type, $$.type);
            int arg3 = new_temp(slist, $$.type);
            gen(table, slist, clist, _inv[$$.type], arg1, -1, arg3);
            $$.addr = arg3;
        }
        | '(' expr ')'
        {
            $$.type = $2.type;
            $$.addr = $2.addr;
        }
        | ID
        {
            symbol *id = find_symbol(slist, $1.name);
            if (id == NULL)
            {
                char msg[MAXLEN];
                snprintf(msg,MAXLEN,"undeclared symbol \"%s\"", $1.name);
                yyerror(msg);
                id = new_symbol(slist, $1.name, INT);
            }
            $$.type = id->type;
            $$.addr = id->addr;
        }
        | CONST
        {
            constant *c = new_constant(clist, $1.val, $1.type);
            $$.type = c->type;
            $$.addr = c->addr;
        }

%%

int main()
{
    table = new_quadtable();
    clist = new_constant_list(CONSTANT_BEGIN);
    yyparse();
    if (error == 0)
    {
        print_constant_list(clist);
        print_quadtable(table);

        FILE *out=fopen("generate.txt","w");
        save_constant_list(out,clist);
        save_quadtable(out,table);
        fclose(out);
    }
    else
        printf("%d error generated.\n", error);
    delete_constant_list(clist);
    delete_quadtable(table);
    return 0;
}
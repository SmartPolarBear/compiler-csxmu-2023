%option noyywrap
%{
#include <stdlib.h>
#include <stdbool.h>

#include "types.h"
#include "y.tab.h"

int line = 1;
%}

space   (\ |\t)+
id      [A-Za-z][0-9A-Za-z]*
num     [0-9]+
real    [0-9]+(\.[0-9]+)?(E[+-]?[0-9]+)?
char    '.'

%%

{space} {}
"\n"    { line++; }

if      { return IF; }
else    { return ELSE; }
do      { return DO; }
while   { return WHILE; }
break   { return BREAK; }

int     { yylval.type = INT; return BASIC; }
float   { yylval.type = FLOAT; return BASIC; }
char    { yylval.type = CHAR; return BASIC; }
bool    { yylval.type = BOOL; return BASIC; }

true    { yylval.type = BOOL; yylval.val.boolean = true; return CONST; }
false   { yylval.type = BOOL; yylval.val.boolean = false; return CONST; }
{num}   { yylval.type = INT; yylval.val.num = atoi(yytext); return CONST; }
{real}  { yylval.type = FLOAT; yylval.val.real = atof(yytext); return CONST; }
{char}  { yylval.type = CHAR; yylval.val.ch = yytext[1]; return CONST; }

{id}    { strcpy(yylval.name, yytext); return ID; }

"=="    { return EQ; }
"!="    { return NE; }
"<"     { return LT; }
"<="    { return LE; }
">"     { return GT; }
">="    { return GE; }
"&&"    { return AND; }
"||"    { return OR; }

"!"     { return '!'; }
"="     { return '='; }
"+"     { return '+'; }
"-"     { return '-'; }
"*"     { return '*'; }
"/"     { return '/'; }
";"     { return ';'; }
"("     { return '('; }
")"     { return ')'; }
"["     { return '['; }
"]"     { return ']'; }
"{"     { return '{'; }
"}"     { return '}'; }

%%

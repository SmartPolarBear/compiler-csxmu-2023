%{
#include <stdio.h>
#include <stdlib.h>

#include "lexer.h"

#define EMIT_TOKEN(type) yylval=create_token(type,yytext,yylineno); return (type);

struct token* yylval;
int yylines=0;

static void yyerror(char *msg)
{
    fprintf(stderr, "Error: %s", msg);
    exit(-1);
}

%}

delim    [ \t\r\n]
ws       {delim}+
letter   [A-Za-z]
digit    [0-9]
id       {letter}({letter}|{digit})*
number   {digit}+(\.{digit}+)?(E[+-]?{digit}+)?
eoc      \*\/

%x COMMENT
%x STRING

%%
%{
   int _yycmtnest = 0;
%}
"/*"            { BEGIN(COMMENT); ++_yycmtnest;}
"//".*          /* // comments to end of line */
<COMMENT>[^*/]* /* Eat non-comment delimiters */
<COMMENT>"/*"   {++_yycmtnest;}
<COMMENT>"*/"   {if (--_yycmtnest == 0) BEGIN(INITIAL);}
<COMMENT>[*/]   /* Eat a / or * if it doesn't match comment sequence */
<COMMENT><<EOF>> { yyerror("EOF in comment."); }
{eoc}            { yyerror("Stray */ symbol in code."); }

["]             { BEGIN(STRING);}
<STRING>[^"]*   { EMIT_TOKEN(TK_STRING); }
<STRING>["]     { BEGIN(INITIAL);}
<STRING><<EOF>> { yyerror("EOF in string."); }

{ws}       { /*no action and no return */ }
if         { EMIT_TOKEN(TK_IF); }
while      { EMIT_TOKEN(TK_WHILE);}
do         { EMIT_TOKEN(TK_DO);}
break      { EMIT_TOKEN(TK_BREAK);}
true       { EMIT_TOKEN(TK_TRUE); }
false      { EMIT_TOKEN(TK_FALSE);}
int        { EMIT_TOKEN(TK_INT);  }
char       { EMIT_TOKEN(TK_CHAR);  }
bool       { EMIT_TOKEN(TK_BOOL); }
float      { EMIT_TOKEN(TK_FLOAT); }

{id}       { EMIT_TOKEN(TK_ID); }
{number}   { EMIT_TOKEN(TK_NUM); }

\(         { EMIT_TOKEN(TK_LPAREN);}
\)         { EMIT_TOKEN(TK_RPAREN);}
\]         { EMIT_TOKEN(TK_LBRACKET); }
\[         { EMIT_TOKEN(TK_RBRACKET); }
\{         { EMIT_TOKEN(TK_LBRACE); }
\}         { EMIT_TOKEN(TK_RBRACE); }


";"        { EMIT_TOKEN(TK_SEMICOLON); }
","        { EMIT_TOKEN(TK_COMMA); }
"+"        { EMIT_TOKEN(TK_PLUS); }
"-"        { EMIT_TOKEN(TK_MINUS); }
"*"        { EMIT_TOKEN(TK_MULTIPLY); }
"/"        { EMIT_TOKEN(TK_DIVIDE); }
"<"        { EMIT_TOKEN(TK_LT); }
"<="       { EMIT_TOKEN(TK_LTE); }
"="        { EMIT_TOKEN(TK_EQ); }
"!="       { EMIT_TOKEN(TK_NEQ); }
">"        { EMIT_TOKEN(TK_GT); }
">="       { EMIT_TOKEN(TK_GTE); }

.          { yyerror("Unexpected character.");}
%%

int yywrap()
{
    return 1;
}

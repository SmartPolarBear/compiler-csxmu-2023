%{
#include <stdio.h>
#include <stdlib.h>

#include "lexer.h"

#define ID_TKN      500
#define NUM_TKN     510

#define RELOP_TKN   520
#define RELOP_AD    521
#define RELOP_SU    522
#define RELOP_MU    523
#define RELOP_DI    524
#define RELOP_LT    525
#define RELOP_LE    526
#define RELOP_EQ    527
#define RELOP_NE    528
#define RELOP_GT    529
#define RELOP_GE    530
#define RELOP_SENI    531
#define RELOP_COMMA    532

#define LEFT_BRACKET  530
#define RIGHT_BRACKET 531
#define LEFT_MIBRACKET  532
#define RIGHT_MIBRACKET 533
#define LEFT_BIBRACKET  534
#define RIGHT_BIBRACKET 535


#define IF_TKN       610
#define WHILE_TKN    611
#define DO_TKN       612
#define BREAK_TKN    613
#define REAL_TKN     614
#define TRUE_TKN     615
#define FALSE_TKN    616
#define BASIC_TKN    617

int yylval;

%}

delim    [" "\t\n]
ws       {delim}+
letter   [A-Za-z]
digit    [0-9]
id       {letter}({letter}|{digit})*
number   {digit}+(\.{digit}+)?(E[+-]?{digit}+)?
explan   \/\*.*\*\/
%%

{ws}       {/*no action and no return */ }
if         { printf("IF:%s\n",yytext);   return (IF_TKN); }
while       { printf("WHILE:%s\n",yytext); return (WHILE_TKN); }
do    { printf("DO:%s\n",yytext); return (DO_TKN); }
break    { printf("BREAK:%s\n",yytext); return (BREAK_TKN); }
real    { printf("REAL:%s\n",yytext); return (REAL_TKN); }
true      { printf("TRUE:%s\n",yytext); return (TRUE_TKN); }
false     { printf("FALSE:%s\n",yytext); return (FALSE_TKN); }
int       { printf("BASIC:%s\n",yytext); return (BASIC_TKN); }
char      { printf("BASIC:%s\n",yytext); return (BASIC_TKN); }
bool      { printf("BASIC:%s\n",yytext); return (BASIC_TKN); }
float     { printf("BASIC:%s\n",yytext); return (BASIC_TKN); }


{explan}    {printf("REMARK:%s\n",yytext);}
{id}       { yylval = (int) install_id(); printf("ID:%s\n",yytext); return (ID_TKN); }
{number}   { yylval = (int) install_num();printf("Num:%d\n",yylval); return (NUM_TKN); }

\(         { yylval = LEFT_BRACKET; printf("LEFT_BRACKET:%s\n",yytext);  return (LEFT_BRACKET); }
\)         { yylval = RIGHT_BRACKET; printf("RIGHT_BRACKET:%s\n",yytext);  return (RIGHT_BRACKET); }
\]         { yylval = LEFT_MIBRACKET; printf("LEFT_MIBRACKET:%s\n",yytext);  return (LEFT_MIBRACKET); }
\[         { yylval = RIGHT_MIBRACKET; printf("RIGHT_MIBRACKET:%s\n",yytext);  return (RIGHT_MIBRACKET); }
\{         { yylval = LEFT_BIBRACKET; printf("LEFT_BIBRACKET:%s\n",yytext);  return (LEFT_BIBRACKET); }
\}         { yylval = RIGHT_BIBRACKET; printf("RIGHT_BIBRACKET:%s\n",yytext);  return (RIGHT_BIBRACKET); }


";"       { yylval = RELOP_SENI; printf("SENI:%s\n",yytext);  return(RELOP_TKN); }
","       { yylval = RELOP_COMMA; printf("COMMA:%s\n",yytext);  return(RELOP_TKN); }
"+"        { yylval = RELOP_AD; printf("AD:%s\n",yytext);  return(RELOP_TKN); }
"-"        { yylval = RELOP_SU; printf("SU:%s\n",yytext);  return(RELOP_TKN); }
"*"        { yylval = RELOP_MU; printf("MU:%s\n",yytext);  return(RELOP_TKN); }
"/"        { yylval = RELOP_DI; printf("DI:%s\n",yytext);  return(RELOP_TKN); }
"<"        { yylval = RELOP_LT; printf("LT:%s\n",yytext);  return(RELOP_TKN); }
"<="       { yylval = RELOP_LE; printf("LE:%s\n",yytext);  return(RELOP_TKN); }
"="        { yylval = RELOP_EQ; printf("EQ:%s\n",yytext);  return(RELOP_TKN); }
"!="       { yylval = RELOP_NE; printf("NE:%s\n",yytext);  return(RELOP_TKN); }
">"        { yylval = RELOP_GT; printf("GT:%s\n",yytext);  return(RELOP_TKN); }
">="       { yylval = RELOP_GE; printf("GE:%s\n",yytext);  return(RELOP_TKN); }


%%

int yywrap()/*多文件*/
{
    return 1;
}

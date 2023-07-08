%option noyywrap
%{
#include<stdlib.h>
#include<ctype.h>
#include<stdio.h>
%}
%%
"if" {return IF;} 
"while" {return WHILE;} 
"do" {return DO;} 
"break" {return BREAK;}
"int"|"float"|"bool"|"char" {return BASIC;}
"true" {return TRUE;}
"false" {return FALSE;}
"else"  {return ELSE;}
[a-zA-Z_][a-zA-Z_0-9]* {return ID;}
[0-9]+\.[0-9]+ {return REAL;}
[0-9]+ {return NUM;}
"||" {return OR;}
"&&" {return AND;}
"|" {return '|';}
"&" {return '&';}
"<=" {return LE;}
"<" { return '<';}
">=" {return GE;}
">" {return '>';}
"!=" {return NE;}
"=" { return '=';}
"==" {return EQ;}
"\+"  {return '+';}
"\-" {return '-';}
"\*" {return '*';}
"\/" {return '/';}
"(" {return '(';}
")" {return ')';}
";" {return ';';}
"{" {return '{';}
"}" {return '}';}
"[" {return '['; }
"]" {return ']';}
"//".*  {}
[ \n\t] {}
%%
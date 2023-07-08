%{
#include<ctype.h>
#include<stdio.h>
%}
%left '+' '-'
%left '*' '/'
%nonassoc UMINUS
%token NUM AND BASIC BREAK DO ELSE EQ FALSE GE
%token ID IF INDEX LE MINUS NE OR REAL TRUE WHILE
KET
 %token  RIGHT_BRA
%%
program : block { printf("program-->block\n"); }
        ;
block : '{' decls stmts '}' { printf("block-->{decls stmts}\n"); }
      ;
decls : decls decl { printf("decls-->decls decl\n"); }
      | /*empty*/
      ;
decl : type ID ';' { printf("decl-->type id;\n"); }
     ;
type : type '[' NUM ']' { printf("type-->type[num]\n"); }
     | BASIC { printf("type-->basic\n"); }
     ;
stmts : stmts stmt { printf("stmts-->stmts stmt\n"); }
     | /*empty*/
     ;
stmt : DO stmt WHILE '(' bool ')' ';' { printf("stmt-->do stmt while(bool);\n"); }
     | IF '(' bool ')' stmt { printf("stmt--if(bool)stmt\n"); }
     | IF '(' bool ')' stmt ELSE stmt { printf("stmt-->if(bool)stmt else stmt\n"); }
     | WHILE '(' bool ')' stmt { printf("stmt-->while(bool)stmt\n"); }
     | BREAK ';' { printf("stmt-->break;\n"); }
     | block { printf("stmt-->block\n"); }
     | ID '=' bool ';' { printf("stmt-->id=bool;\n"); }
     ;

bool : bool OR join { printf("bool-->bool||join\n"); }
     | join { printf("bool-->join\n"); }
     ;
join : join AND equality { printf("join-->join&&equality\n"); }
     | equality { printf("join-->equality\n"); }
     ;
equality : equality EQ rel { printf("equality-->equality==rel\n"); }
         | equality NE rel { printf("equality-->equality!=rel\n"); }
         | rel { printf("equality-->rel\n"); }
         ;
rel : expr '<' expr { printf("rel-->expr<expr\n"); }
    | expr LE expr { printf("rel-->expr<=expr\n"); } 
    | expr GE expr { printf("rel-->expr>=expr\n"); }
    | expr '>' expr { printf("rel-->expr>expr\n"); }
    | expr { printf("rel-->expr\n"); }
    ;
expr : expr '+' term { printf("expr-->expr+term\n"); }
     | expr '-' term { printf("expr-->expr-term\n"); }  
     | term { printf("expr-->term\n"); }
     ;
term : term '*' unary { printf("term-->term*unary\n"); }
     | term '/' unary { printf("term-->term/unary\n"); }
     | unary { printf("term-->unary\n"); }
     ;
unary : '!' unary { printf("unary-->!unary\n"); }
      | '-' unary %prec UMINUS{ printf("unary-->-unary\n"); }
      | factor { printf("unary-->factor\n"); }
      ;
factor : '(' bool ')' { printf("factor-->(bool)\n"); }
       | ID { printf("factor-->id\n"); }
       | NUM { printf("factor-->num\n"); }
       | REAL { printf("factor-->real\n"); }
       | TRUE { printf("factor-->true\n"); }
       | FALSE { printf("factor-->false\n"); }
       ;
       
%%
#include "lex.yy.c"

int  main()
{
 extern FILE *yyin;
 yyin=fopen("b.txt","r");
 yyparse();
 return 0;
}
yyerror(char *s)
{
    printf("%s error!\n",s);
}
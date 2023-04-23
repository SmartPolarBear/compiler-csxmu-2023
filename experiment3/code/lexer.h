#pragma once

#ifdef __cplusplus
#include <cstdio>
#else
#include <stdio.h>
#endif

#ifdef __cplusplus
extern "C"
{
#endif

extern FILE *yyin;
extern int yylex(void);
extern struct token *yylval;

enum token_type
{
	// reserved words
	TK_IF = 1,
	TK_ELSE,
	TK_RETURN,
	TK_WHILE,
	TK_DO,
	TK_BREAK,
	TK_TRUE,
	TK_FALSE,
	TK_VOID,
	TK_INT,
	TK_CHAR,
	TK_FLOAT,
	TK_BOOL,
	// special symbols
	TK_PLUS,
	TK_DPLUS,
	TK_MINUS,
	TK_DMINUS,
	TK_MULTIPLY,
	TK_POWER,
	TK_DIVIDE,
	TK_LT,
	TK_LTE,
	TK_GT,
	TK_GTE,
	TK_EQ,
	TK_NOT,
	TK_NEQ,
	TK_ASSIGN,
	TK_SEMICOLON,
	TK_COMMA,
	TK_LPAREN,
	TK_RPAREN,
	TK_LBRACKET,
	TK_RBRACKET,
	TK_LBRACE,
	TK_RBRACE,
	TK_QUESTION,
	TK_COLON,
	// identifiers
	TK_ID,
	// constants
	TK_NUM,
	TK_STRING,
};

void *create_token(enum token_type t, const char *s, int l);

void consume_comment(int (*input)());

#ifdef __cplusplus
}
#endif



//
// Created by bear on 3/24/2023.
//

#pragma once

#include <string>

// token type enum as defined in the dragon book
enum class token_type
{
	// reserved words
	IF,
	ELSE,
	INT,
	RETURN,
	WHILE,
	VOID,
	// special symbols
	PLUS,
	DPLUS,
	MINUS,
	DMINUS,
	MULTIPLY,
	POWER,
	DIVIDE,
	LT,
	LTE,
	GT,
	GTE,
	EQ,
	NOT,
	NEQ,
	ASSIGN,
	SEMICOLON,
	COMMA,
	LPAREN,
	RPAREN,
	LBRACKET,
	RBRACKET,
	LBRACE,
	RBRACE,
	// identifiers
	ID,
	// constants
	NUM,
	STRING,
	// end of file
	EOF_TOKEN
};

struct token
{
	token_type type;
	std::string lexeme;
	int line;
};
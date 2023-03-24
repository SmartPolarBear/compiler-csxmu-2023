//
// Created by bear on 3/24/2023.
//

#pragma once

#include <string>
#include <optional>
#include <variant>
#include <format>

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

static inline constexpr std::string token_type_name(token_type t)
{
	// convert t to its name in string
	switch (t)
	{
	case token_type::IF:
		return "IF";
	case token_type::ELSE:
		return "ELSE";
	case token_type::INT:
		return "INT";
	case token_type::RETURN:
		return "RETURN";
	case token_type::WHILE:
		return "WHILE";
	case token_type::VOID:
		return "VOID";
	case token_type::PLUS:
		return "PLUS";
	case token_type::DPLUS:
		return "DPLUS";
	case token_type::MINUS:
		return "MINUS";
	case token_type::DMINUS:
		return "DMINUS";
	case token_type::MULTIPLY:
		return "MULTIPLY";
	case token_type::POWER:
		return "POWER";
	case token_type::DIVIDE:
		return "DIVIDE";
	case token_type::LT:
		return "LT";
	case token_type::LTE:
		return "LTE";
	case token_type::GT:
		return "GT";
	case token_type::GTE:
		return "GTE";
	case token_type::EQ:
		return "EQ";
	case token_type::NOT:
		return "NOT";
	case token_type::NEQ:
		return "NEQ";
	case token_type::ASSIGN:
		return "ASSIGN";
	case token_type::SEMICOLON:
		return "SEMICOLON";
	case token_type::COMMA:
		return "COMMA";
	case token_type::LPAREN:
		return "LPAREN";
	case token_type::RPAREN:
		return "RPAREN";
	case token_type::LBRACKET:
		return "LBRACKET";
	case token_type::RBRACKET:
		return "RBRACKET";
	case token_type::LBRACE:
		return "LBRACE";
	case token_type::RBRACE:
		return "RBRACE";
	case token_type::ID:
		return "ID";
	case token_type::NUM:
		return "NUM";
	case token_type::STRING:
		return "STRING";
	case token_type::EOF_TOKEN:
		return "EOF_TOKEN";
	default:
		return "UNKNOWN";
	}
	return "UNKNOWN";
}

using literal_type = std::optional<std::variant<int, double, std::string>>;

struct token
{
	token_type type;
	std::string lexeme;
	int line;
	literal_type literal;
};

struct literal_printer
{
	std::string operator()(int i) const
	{ return std::format("[Integer type, literal: {}]", std::to_string(i)); }
	std::string operator()(double d) const
	{ return std::format("[Floating type, literal: {}]", std::to_string(d)); }
	std::string operator()(std::string s) const
	{ return std::format("[String type, literal: {}]", s); }
};
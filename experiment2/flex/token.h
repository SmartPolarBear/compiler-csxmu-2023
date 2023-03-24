//
// Created by bear on 3/24/2023.
//

#pragma once

//
// Created by bear on 3/24/2023.
//

#pragma once

#include <string>
#include <optional>
#include <variant>

static inline std::string token_type_name(token_type t)
{
	// convert t to its name in string
	switch (t)
	{
	case token_type::TK_IF:
		return "IF";
	case token_type::TK_ELSE:
		return "ELSE";
	case token_type::TK_INT:
		return "INT";
	case token_type::TK_RETURN:
		return "RETURN";
	case token_type::TK_WHILE:
		return "WHILE";
	case token_type::TK_VOID:
		return "VOID";
	case token_type::TK_PLUS:
		return "PLUS";
	case token_type::TK_DPLUS:
		return "DPLUS";
	case token_type::TK_MINUS:
		return "MINUS";
	case token_type::TK_DMINUS:
		return "DMINUS";
	case token_type::TK_MULTIPLY:
		return "MULTIPLY";
	case token_type::TK_POWER:
		return "POWER";
	case token_type::TK_DIVIDE:
		return "DIVIDE";
	case token_type::TK_LT:
		return "LT";
	case token_type::TK_LTE:
		return "LTE";
	case token_type::TK_GT:
		return "GT";
	case token_type::TK_GTE:
		return "GTE";
	case token_type::TK_EQ:
		return "EQ";
	case token_type::TK_NOT:
		return "NOT";
	case token_type::TK_NEQ:
		return "NEQ";
	case token_type::TK_ASSIGN:
		return "ASSIGN";
	case token_type::TK_SEMICOLON:
		return "SEMICOLON";
	case token_type::TK_COMMA:
		return "COMMA";
	case token_type::TK_LPAREN:
		return "LPAREN";
	case token_type::TK_RPAREN:
		return "RPAREN";
	case token_type::TK_LBRACKET:
		return "LBRACKET";
	case token_type::TK_RBRACKET:
		return "RBRACKET";
	case token_type::TK_LBRACE:
		return "LBRACE";
	case token_type::TK_RBRACE:
		return "RBRACE";
	case token_type::TK_ID:
		return "ID";
	case token_type::TK_NUM:
		return "NUM";
	case token_type::TK_STRING:
		return "STRING";
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

//struct literal_printer
//{
//	std::string operator()(int i) const
//	{ return std::format("[Integer type, literal: {}]", std::to_string(i)); }
//	std::string operator()(double d) const
//	{ return std::format("[Floating type, literal: {}]", std::to_string(d)); }
//	std::string operator()(std::string s) const
//	{ return std::format("[String type, literal: {}]", s); }
//};

//
// Created by bear on 3/24/2023.
//

#include <format>
#include <iostream>

#include "lexer.h"

using namespace std;

std::vector<token> scanner::scan()
{
	while (!is_end())
	{
		start_ = current_;
		scan_next();
	}

	tokens_.emplace_back(token_type::EOF_TOKEN, "", line_);
	return tokens_;
}

void scanner::scan_next()
{
	char c = advance();
	switch (c)
	{
	case '(':
		add_token(token_type::LPAREN);
		break;
	case ')':
		add_token(token_type::RPAREN);
		break;
	case '[':
		add_token(token_type::LBRACKET);
		break;
	case ']':
		add_token(token_type::RBRACKET);
		break;
	case '{':
		add_token(token_type::LBRACE);
		break;
	case '}':
		add_token(token_type::RBRACE);
		break;
	case ',':
		add_token(token_type::COMMA);
		break;
	case '-':
		if (match('-'))
		{
			add_token(token_type::DMINUS);
		}
		else
		{
			add_token(token_type::MINUS);
		}
		break;
	case '+':
		if (match('+'))
		{
			add_token(token_type::DPLUS);
		}
		else
		{
			add_token(token_type::PLUS);
		}
		break;
	case ';':
		add_token(token_type::SEMICOLON);
		break;
	case '*':
		if (match('*'))
		{
			add_token(token_type::POWER);
		}
		else
		{
			add_token(token_type::MULTIPLY);
		}
		break;
	case '!':
		add_token(match('=') ? token_type::NEQ : token_type::NOT);
		break;
	case '=':
		add_token(match('=') ? token_type::EQ : token_type::ASSIGN);
		break;
	case '<':
		add_token(match('=') ? token_type::LTE : token_type::LT);
		break;
	case '>':
		add_token(match('=') ? token_type::GTE : token_type::GT);
		break;

	case '/':
		if (match('/')) // this is a line comment
		{
			consume_line_comment();
		}
		else if (match('*')) // this is a block comment
		{
			consume_block_comment();
		}
		else
		{
			add_token(token_type::DIVIDE);
		}
		break;

	case ' ':
	case '\r':
	case '\t':
		// Do nothing to ignore whitespaces.
		break;

	case '\n':
		line_++;
		break;

	case '"':
		scan_string();
		break;

	default:
		if (is_number_literal_component(c))
		{
			scan_number_literal();
		}
		else if (is_identifier_component(c))
		{
			scan_identifier();
		}
		else
		{
			cerr << format("Unexpected character {}.", c) << endl;
		}

		break;
	}
}

void scanner::scan_string()
{
	while (peek() != '"' && !is_end())
	{
		if (peek() == '\n')
		{
			line_++;
		}
		advance();
	}

	if (is_end())
	{
		cerr << "Unterminated string." << endl;
		return;
	}

	advance(); // eat the closing "

	auto lexeme = whole_lexeme();
	add_token(token_type::STRING, lexeme.substr(1, lexeme.size() - 2));
}

void scanner::scan_number_literal()
{
	while (is_number_literal_component(peek()))
		advance();

	bool floating{false};
	if (peek() == '.' && is_number_literal_component(peek(1)))
	{
		floating = true;
		advance();
		while (is_number_literal_component(peek()))
			advance();
	}

	if (floating)
	{
		add_token(token_type::NUM, stod(whole_lexeme()));
	}
	else
	{
		add_token(token_type::NUM, stoi(whole_lexeme()));
	}
}

void scanner::scan_identifier()
{
	while (is_identifier_component(peek()))
		advance();

	auto text = whole_lexeme();
	auto keyword = keywords_to_type_.find(text);

	if (keyword != keywords_to_type_.end())
	{
		add_token(keywords_to_type_[text]);
	}
	else
	{
		add_token(token_type::ID);
	}
}

void scanner::consume_block_comment()
{
	while (!is_end())
	{
		auto c = advance();
		if (c == '\n')
		{
			line_++;
		}
		else if (c == '*' && peek() == '/')
		{
			[[maybe_unused]]auto _ = advance(); // eat "/"
			return;
		}
		else if (c == '/' && peek() == '*') // nested block comment
		{
			[[maybe_unused]]auto _ = advance(); // eat "*"
			consume_block_comment();
		}
	}

	// not enough code to find next close sign */, so it's an error
	if (is_end())
	{
		cerr << "Unterminated block comment." << endl;
	}
}

void scanner::consume_line_comment()
{
	while (peek() != '\n' && !is_end())
		advance();
}

void scanner::add_token(token_type t)
{
	tokens_.emplace_back(t, whole_lexeme(), line_);
}

void scanner::add_token(token_type t, const literal_type &lit)
{
	tokens_.emplace_back(t, whole_lexeme(), line_, lit);
}

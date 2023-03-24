//
// Created by bear on 3/24/2023.
//

#pragma once
#include <string>
#include <utility>
#include <unordered_map>
#include <experimental/generator>

#include "token.h"

class scanner
{
public:
	explicit scanner(std::string input)
		: code_(std::move(input)), line_(1), tokens_()
	{
		start_ = code_.begin();
		current_ = code_.begin();
	}

	std::vector<token> scan();
private:
	std::unordered_map<std::string, token_type> keywords_to_type_{
		{"if", token_type::IF},
		{"else", token_type::ELSE},
		{"int", token_type::INT},
		{"return", token_type::RETURN},
		{"while", token_type::WHILE},
		{"void", token_type::VOID}
	};

	void add_token(token_type t);

	void scan_string();

	void scan_number_literal();

	void scan_identifier();

	void consume_block_comment();

	void consume_line_comment();

	void scan_next();

	bool is_end()
	{
		return current_ == code_.end();
	}

	char advance()
	{
		return *current_++;
	}

	char peek()
	{
		if (is_end())
		{
			return 0;
		}
		return *current_;
	}

	char peek(int n)
	{
		if (current_ + n >= code_.end())
		{
			return 0;
		}
		return *(current_ + n);
	}

	bool match(char expected)
	{
		if (is_end())
		{
			return false;
		}
		if (*current_ != expected)
		{
			return false;
		}
		current_++;
		return true;
	}

	std::string whole_lexeme()
	{
		return {start_, current_};
	}

	static inline constexpr bool is_number(char c)
	{
		return c >= '0' && c <= '9';
	}

	static inline constexpr bool is_letter(char c)
	{
		return (c >= 'a' && c <= 'z') ||
			(c >= 'A' && c <= 'Z') ||
			c == '_';
	}

	static inline constexpr bool is_number_literal_component(char c)
	{
		return is_number(c);
	}

	static inline constexpr bool is_identifier_component(char c)
	{
		return is_number(c) || is_letter(c);
	}

	std::string code_;
	std::string::iterator current_, start_;
	int line_;
	std::vector<token> tokens_;
};
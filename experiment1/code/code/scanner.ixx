//
// Created by bear on 3/10/2023.
//

module;
#include <string>
#include <cctype>
#include <experimental/generator>
#include <stdexcept>

export module scanner;

export enum class token_type
{
	NUM = 1, ID, PLUS, MINUS, STAR, DIVISION, LPAREN, RPAREN
};

export struct token
{
	token_type type;
	std::string lexeme;
};

export std::experimental::generator<token> scan(std::string code)
{
	auto it = code.begin();
	while (it != code.end())
	{
		if (*it == ' ' || *it == '\t' || *it == '\n' || *it == '\r')
		{
			++it;
		}
		else if (std::isdigit(*it))
		{
			auto peek = it;
			while (std::isdigit(*peek))
			{
				++peek;
			}
			token t{};
			t.type = token_type::NUM;
			t.lexeme = std::string(it, peek);
			it = peek;
			co_yield t;
		}
		else if (std::isalpha(*it))
		{
			auto peek = it;
			while (peek != code.end() && std::isalpha(*peek))
			{
				++peek;
			}
			token t{};
			t.type = token_type::ID;
			t.lexeme = std::string(it, peek);
			it = peek;
			co_yield t;
		}
		else if (*it == '+' || *it == '-' || *it == '*' || *it == '/')
		{
			token t{};
			if (*it == '+')
			{
				t.type = token_type::PLUS;
			}
			else if (*it == '-')
			{
				t.type = token_type::MINUS;
			}
			else if (*it == '*')
			{
				t.type = token_type::STAR;
			}
			else if (*it == '/')
			{
				t.type = token_type::DIVISION;
			}
			t.lexeme = std::string(it, it + 1);
			it++;
			co_yield t;
		}
		else if (*it == '(' || *it == ')')
		{
			token t{};
			t.type = *it == '(' ? token_type::LPAREN : token_type::RPAREN;
			t.lexeme = std::string(it, it + 1);
			it++;
			co_yield t;
		}
		else
		{
			throw std::runtime_error("Invalid token");
		}
	}
	co_return;
}

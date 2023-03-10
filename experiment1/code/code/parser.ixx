//
// Created by bear on 3/10/2023.
//
module;

#include <experimental/generator>
#include <vector>
#include <string>
#include <stdexcept>

export module parser;

export import scanner;

export class parser
{
public:
	using token_list = std::experimental::generator<token>;

	explicit parser(std::experimental::generator<token>&& tokens) : tokens_(std::move(tokens))
	{
		current_ = tokens_.begin();
	}

	std::string parse()
	{
		return expr();
	}

private:
	bool peek(token_type t)
	{
		if (current_ != tokens_.end() && current_->type == t)
		{
			return true;
		}
		return false;
	}

	void match(token_type t)
	{
		if (current_->type == t)
		{
			current_++;
		}
		else
		{
			throw std::runtime_error("Unexpected token.");
		}
	}

	std::string factor()
	{
		if (peek(token_type::NUM))
		{
			auto ret = " " + current_->lexeme;
			match(token_type::NUM);
			return ret + " ";
		}
		else if (peek(token_type::ID))
		{
			auto ret = " " + current_->lexeme;
			match(token_type::ID);
			return ret + " ";
		}
		else if (peek(token_type::LPAREN))
		{
			match(token_type::LPAREN);
			auto e = expr();
			match(token_type::RPAREN);
			return e;
		}
		else
		{
			throw std::runtime_error("Expected operand");
		}
	}

	std::string term()
	{
		auto ret = factor();

		while (true)
		{
			if (peek(token_type::STAR))
			{
				match(token_type::STAR);
				ret += factor();
				ret += " * ";
			}
			else if (peek(token_type::DIVISION))
			{
				match(token_type::DIVISION);
				ret += factor();
				ret += " / ";
			}
			else
			{
				return ret;
			}
		}
		return ret;
	}

	std::string expr()
	{
		auto ret = term();

		while (true)
		{
			if (peek(token_type::PLUS))
			{
				match(token_type::PLUS);
				ret += term();
				ret += " + ";
			}
			else if (peek(token_type::MINUS))
			{
				match(token_type::MINUS);
				ret += term();
				ret += " - ";
			}
			else
			{
				return ret;
			}
		}
		return ret;
	}

	token_list tokens_;
	token_list::iterator current_;
};
#include <iostream>
#include <string>
#include <experimental/generator>
#include <vector>
#include <stdexcept>

using namespace std;

enum class token_type
{
	NUM = 1, ID, PLUS, MINUS, STAR, DIVISION, LPAREN, RPAREN
};

struct token
{
	token_type type;
	std::string lexeme;
};


std::experimental::generator<token> scan(std::string code)
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
			while (peek != code.end() && std::isdigit(*peek))
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



class parser
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
			throw std::runtime_error("Expected operand.");
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

int main()
{
	string code;
	cin >> code;
	parser p{ scan(code) };

	try {
		cout << p.parse();
	}
	catch (const std::runtime_error& e) {
		cerr << e.what() << endl;
	}

	return 0;
}
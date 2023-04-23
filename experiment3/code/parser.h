//
// Created by bear on 4/21/2023.
//

#pragma once

#include <string_view>
#include <vector>
#include <limits>
#include <memory>
#include <functional>

#include "lexer.h"
#include "token.h"

class parse_error : public std::runtime_error
{
public:
	parse_error(token *near, std::string_view msg) : near_(near), std::runtime_error(msg.data())
	{
	}

	int line() const
	{
		return near_->line;
	}
private:
	token *near_;
};

class parser
{
public:
	explicit parser(std::string_view src);
	~parser();

	void parse();
private:
	void program();

	void block();

	void stmts();

	void stmt();

	void if_stmt();

	void do_while_stmt();

	void break_stmt();

	void assign_stmt();

	void expr_stmt();

	int grouping();

	int unary();

	int binary(int);


	enum precedence : uint32_t
	{
		PREC_NONE,
		PREC_ASSIGNMENT,  // =
		PREC_OR,          // or
		PREC_AND,         // and
		PREC_EQUALITY,    // == !=
		PREC_COMPARISON,  // < > <= >=
		PREC_TERM,        // + -
		PREC_FACTOR,      // * /
		PREC_UNARY,       // ! -
		PREC_PRIMARY
	};

	void all_expr(precedence prec = PREC_ASSIGNMENT);

	using prefix_parse_fn = decltype(&parser::unary);
	using infix_parse_fn = decltype(&parser::binary);

	struct rule
	{
		precedence pred;
		prefix_parse_fn prefix;
		infix_parse_fn infix;
	};

	std::unordered_map<token_type, rule> expr_rules{
		{token_type::TK_LPAREN, rule{PREC_NONE, &parser::grouping, nullptr}},
		{token_type::TK_PLUS, rule{PREC_TERM, &parser::unary, &parser::binary}},
		{token_type::TK_MINUS, rule{PREC_TERM, &parser::unary, &parser::binary}},
		{token_type::TK_DIVIDE, rule{PREC_FACTOR, nullptr, &parser::binary}},
		{token_type::TK_MULTIPLY, rule{PREC_FACTOR, nullptr, &parser::binary}},
		{token_type::TK_NEQ,rule{PREC_EQUALITY, nullptr, &parser::binary}},
		{token_type::TK_EQ,rule{PREC_EQUALITY, nullptr, &parser::binary}},
		{token_type::TK_GT,rule{PREC_COMPARISON, nullptr, &parser::binary}},
		{token_type::TK_GTE,rule{PREC_COMPARISON, nullptr, &parser::binary}},
		{token_type::TK_LT,rule{PREC_COMPARISON, nullptr, &parser::binary}},
		{token_type::TK_LTE,rule{PREC_COMPARISON, nullptr, &parser::binary}},
		{token_type::TK_NUM, rule{PREC_NONE, &parser::unary, nullptr}},
		{token_type::TK_ID, rule{PREC_NONE, &parser::unary, nullptr}},
	};

	void synchronize();

	token *consume(token_type t, const std::string &msg);

	parse_error error(token *t, const std::string &msg);

	bool match(token_type t, int64_t next)
	{
		if (check(t, next))
		{
			[[maybe_unused]]auto _ = advance();
			return true;
		}

		return false;
	}

	bool match(token_type t)
	{
		return match(t, 0);
	}

	[[nodiscard]] bool check(token_type t, int64_t next)
	{
		if (is_end())
		{
			return false;
		}
		return peek(next)->type == t;
	}

	[[nodiscard]] bool check(token_type t)
	{
		return check(t, 0);
	}

	[[nodiscard]]token *peek(int64_t offset)
	{
		return tokens_.at(cur_ + offset);
	}

	[[nodiscard]]token *peek()
	{
		return peek(0);
	}

	[[nodiscard]]token *advance()
	{
		if (!is_end())
		{
			cur_++;
		}
		return previous();
	}

	[[nodiscard]]bool is_end()
	{
		return cur_ >= tokens_.size();
	}

	[[nodiscard]]token *previous()
	{
		return tokens_.at(cur_ - 1);
	}

	std::vector<token *> tokens_{};
	int cur_{0};
};

#include <vector>
#include <iostream>

#include "lexer.h"
#include "token.h"
#include "parser.h"

using namespace std;

parser::parser(std::string_view src)
{
	yyin = fopen(src.data(), "r");
	fseek(yyin, 0, SEEK_SET);

	int token = 0;
	literal_printer pt;
	while ((token = yylex()) != 0)
	{
		tokens_.emplace_back(yylval);
	}
}

parser::~parser()
{
	fclose(yyin);
	yyin = nullptr;

	for (auto &t: tokens_)
	{
		delete t;
	}
}

void parser::parse()
{
	program();
}

void parser::program()
{
	cout << "program-> ";
	consume(token_type::TK_LBRACE, "Expect '{'");
	block();
}

void parser::block()
{
	cout << "block" << endl;
	cout << "block->{ stmts }" << endl;
	stmts();
	consume(token_type::TK_RBRACE, "Expect '}'");
}

void parser::stmts()
{
	cout << "stmts-> stmt stmts" << endl;
	while (!is_end())
	{
		stmt();
		if (peek()->type == token_type::TK_RBRACE)
		{
			return;
		}
	}
}

void parser::stmt()
{
	cout << "stmt->";
	if (match(token_type::TK_IF))
	{
		return if_stmt();
	}
	else if (match(token_type::TK_WHILE))
	{
		return do_while_stmt();
	}
	else if (match(token_type::TK_DO))
	{
		return do_while_stmt();
	}
	else if (match(token_type::TK_BREAK))
	{
		return break_stmt();
	}
	else if (match(token_type::TK_LBRACE))
	{
		return block();
	}
	else if (match(token_type::TK_ID))
	{
		return assign_stmt();
	}
	else
	{
		return expr_stmt();
	}
}

// pratt parsing
void parser::all_expr(precedence prec)
{
	// Get the next token
	auto token = advance();

	// Get the prefix parse function for the token
	auto prefix = expr_rules[token->type].prefix;

	// Check if there is a prefix parse function for the token
	if (!prefix)
	{
		throw std::runtime_error("Unexpected token: " + token->lexeme);
	}

	// Parse the prefix expression
	auto left = (this->*prefix)();

	// Loop until the precedence is lower than the current precedence
	while (prec <= expr_rules[peek()->type].pred)
	{
		// Get the infix parse function for the next token
		auto in_tk = advance();
		auto infix = expr_rules[in_tk->type].infix;

		// Parse the infix expression
		left = (this->*infix)(left);
	}

	// Return the parsed expression
//	return left;
}

void parser::synchronize()
{
}

token *parser::consume(token_type t, const string &msg)
{
	if (check(t))
	{
		return advance();
	}
	throw error(peek(), msg);
}

parse_error parser::error(token *t, const string &msg)
{
	return {t, msg};
}

int parser::grouping()
{
	all_expr();
	consume(TK_RPAREN, "Expect ')' after expression.");
	cout << "bool->expr\nexpr->term\nterm->factor\nfactor->(expr)\n";
	return 0;
}

int parser::unary()
{
	auto op = previous();
	if (op->type != TK_ID && op->type != TK_NUM)
	{
		all_expr();
	}

	if (op->type == TK_ID)
	{
		cout << "bool->expr\nexpr->term\nterm->factor\nfactor->unary_op id\n";
	}
	else if (op->type == TK_NUM)
	{
		cout << "bool->expr\nexpr->term\nterm->factor\nfactor->unary_op num\n";
	}
	return 0;
}

int parser::binary(int)
{
	auto op = previous();
	auto rule = expr_rules[op->type];
	all_expr((precedence)(rule.pred + 1));

	if (op->type == TK_PLUS || op->type == TK_MINUS)
	{
		cout << "bool->expr\nexpr->expr" << op->lexeme << "term\n";
	}
	else if (op->type == TK_MULTIPLY || op->type == TK_DIVIDE)
	{
		cout << "bool->expr\nexpr->term\nterm->term" << op->lexeme << "factor\n";
	}
	else
	{
		cout << "bool->expr" << op->lexeme << "expr\n";
	}
	return 0;
}

void parser::if_stmt()
{
	cout << "if(expr)stmt" << endl;
	consume(token_type::TK_LPAREN, "Expect '(' after 'if'");
	all_expr();
	consume(token_type::TK_RPAREN, "Expect ')' after condition");
	stmt();
}

void parser::do_while_stmt()
{
	if (previous()->type == TK_DO)
	{
		cout << "do stmt while(expr)" << endl;
		stmt();
		consume(token_type::TK_WHILE, "Expect 'while' after 'do'");
		consume(token_type::TK_LPAREN, "Expect '(' after 'while'");
		all_expr();
		consume(token_type::TK_RPAREN, "Expect ')' after condition");
		consume(token_type::TK_SEMICOLON, "Expect ';' after 'while'");
	}
	else
	{
		cout << "while(expr)stmt" << endl;
		consume(token_type::TK_LPAREN, "Expect '(' after 'while'");
		all_expr();
		consume(token_type::TK_RPAREN, "Expect ')' after condition");
		stmt();
	}
}

void parser::break_stmt()
{
	cout << "break;" << endl;
	consume(token_type::TK_SEMICOLON, "Expect ';' after 'break'");
}

void parser::assign_stmt()
{
	cout << "assign_stmt" << endl;
	consume(token_type::TK_ASSIGN, "Expect '=' ");
	all_expr();
	consume(token_type::TK_SEMICOLON, "Expect ';'");
}

void parser::expr_stmt()
{
	all_expr();
	consume(token_type::TK_SEMICOLON, "Expect ';'");
}

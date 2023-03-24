#include <string>

#include "lexer.h"
#include "token.h"

using namespace std;

extern "C" void *create_token(token_type t, const char *s, int l)
{
	string lexeme{s};
	if (t != TK_NUM && t != TK_STRING)
	{
		return new token{t, lexeme, l, {}};
	}
	else
	{
		if (t == TK_NUM)
		{
			bool floating = false;
			for (char p: lexeme)
			{
				if (p == '.')
				{
					floating = true;
					break;
				}
			}

			if (floating)
			{
				return new token{t, lexeme, l, {stod(lexeme)}};
			}
			else
			{
				return new token{t, lexeme, l, {stoi(lexeme)}};
			}
		}
		else
		{
			return new token{t, lexeme, l, {lexeme}};
		}
	}
}

extern "C" void consume_comment(int (*input)())
{
	int level = 1;
	char c = input();
	do
	{
		if (c == '*')
		{
			char peek = fgetc(yyin);
			if (peek == '/')
			{
				level--;
			}
			else
			{
				ungetc(peek, yyin);
			}
		}
		else if (c == '/')
		{
			char peek = fgetc(yyin);
			if (peek == '*')
			{
				level++;
			}
			else
			{
				ungetc(peek, yyin);
			}
		}
	} while ((c = fgetc(yyin)) != EOF && (level > 0));
}


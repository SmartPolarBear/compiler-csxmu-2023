#include <iostream>

#include "lexer.h"
#include "token.h"

using namespace std;

int main(int argc, char **argv)
{
	yyin = fopen(argv[1], "r");
	fseek(yyin, 0, SEEK_SET);

	int token = 0;
	literal_printer pt;
	while ((token = yylex()) != 0)
	{
		cout << "Token " << yylval->type
			 << " in line " << yylval->line
			 << ": " << yylval->lexeme;

		if (yylval->literal.has_value())
		{
			cout << ", Value" << std::visit(pt, yylval->literal.value()) << endl;
		}
		else
		{
			cout << endl;
		}
	}

	fclose(yyin);
	yyin = nullptr;

	return 0;
}
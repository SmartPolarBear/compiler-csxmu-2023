#include <iostream>

#include "lexer.h"
#include "token.h"

using namespace std;

int main(int argc, char **argv)
{
	yyin = fopen(argv[1], "r");
	fseek(yyin, 0, SEEK_SET);

	int token = 0;
	while ((token = yylex()) != 0)
	{
		cout << "Token " << yylval->type << " : " << yylval->lexeme << endl;
	}

	fclose(yyin);
	yyin = nullptr;

	return 0;
}
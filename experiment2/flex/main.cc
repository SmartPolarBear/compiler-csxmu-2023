#include "lexer.h"

int main(int argc, char **argv)
{
	yyin = fopen(argv[1], "r");
	fseek(yyin, 0, SEEK_SET);

	int token = 0;
	while ((token = yylex()) != 0);

	fclose(yyin);
	yyin = nullptr;

	return 0;
}
#include <iostream>
#include "parser.h"

using namespace std;

int main(int argc, char **argv)
{
	parser ps{argv[1]};
//	try
//	{
		ps.parse();
//	}
//	catch (const parse_error &e)
//	{
//		cerr << "\nAt Line " << e.line() << ":\n " << e.what() << endl;
//	}
//	int token = 0;
//	literal_printer pt;
//	while ((token = yylex()) != 0)
//	{
//		cout << "Token " << yylval->type
//			 << " in line " << yylval->line
//			 << ": " << yylval->lexeme;
//
//		if (yylval->literal.has_value())
//		{
//			cout << ", Value" << std::visit(pt, yylval->literal.value()) << endl;
//		}
//		else
//		{
//			cout << endl;
//		}
//	}


	return 0;
}
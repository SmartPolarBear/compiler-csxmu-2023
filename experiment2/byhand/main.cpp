#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <format>

#include "lexer.h"

using namespace std;

int main(int argc, char *argv[])
{
	ifstream src{argv[1]};

	stringstream ss{};
	ss << src.rdbuf();

	scanner sc{ss.str()};

	auto tokens = sc.scan();
	cout << std::format("{} Tokens in total:\n", tokens.size());

	for (const auto &t: tokens)
	{
		cout << std::format("  Type: {}, Lexeme: {}, Line: {}", token_type_name(t.type), t.lexeme, t.line);
		if (t.literal.has_value())
		{
			literal_printer pr{};
			cout << std::format(", Value: {}\n", std::visit(pr, t.literal.value()));
		}
		else
		{
			cout << "\n";
		}
	}

	return 0;
}

//
// Created by bear on 3/24/2023.
//

#pragma once

//
// Created by bear on 3/24/2023.
//

#pragma once

#include <string>
#include <optional>
#include <variant>
#include <sstream>
#include "lexer.h"

using literal_type = std::optional<std::variant<int, double, std::string>>;
struct token
{
	token_type type;
	std::string lexeme;
	int line;
	literal_type literal;
};

struct literal_printer
{
	std::string operator()(int i) const
	{
		std::stringstream ss;
		ss << "[Integer type, literal: " << i << "]";
		return ss.str();
	}
	std::string operator()(double d) const
	{
		std::stringstream ss;
		ss << "[Floating type, literal: " << d << "]";
		return ss.str();
	}
	std::string operator()(std::string s) const
	{
		std::stringstream ss;
		ss << "[String type, literal: " << s << "]";
		return ss.str();
	}
};

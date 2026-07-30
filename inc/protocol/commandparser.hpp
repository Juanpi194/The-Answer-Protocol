#pragma once
#include <string>

#include "protocol/command.hpp"

class CommandParser
{
	public:
		static Command	parse(const std::string& line);
};
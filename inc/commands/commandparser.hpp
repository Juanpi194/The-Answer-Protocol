#pragma once
#include <string>

#include "commands/command.hpp"

class CommandParser
{
	public:
		static Command	parse(const std::string& line);
};
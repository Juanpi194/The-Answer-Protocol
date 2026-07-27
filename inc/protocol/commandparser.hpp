#ifndef COMMANDPARSER_HPP
#define COMMANDPARSER_HPP

#include <string>

#include "protocol/command.hpp"

class CommandParser
{
	public:
		Command	parse(const std::string& line) const;
};

#endif
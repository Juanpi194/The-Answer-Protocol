#include <iostream>

#include "utils/utils.hpp"


void	log(const std::string& msg, LogLevel level)
{
	const char	*labels[] = {"", "ERROR", "WARN", "INFO", "DEBUG"};

	if (level <= current_level)
		std::cerr << "[" << labels[level] << "]" << msg << "\n";
}

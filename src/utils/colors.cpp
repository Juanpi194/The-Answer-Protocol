#include "utils/utils.hpp"


std::string	color_str(const std::string& str, Color color)
{
	return "\033[" + std::to_string(color) + "m" + str + "\033[0m";
}

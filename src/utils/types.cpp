#include "utils/types.hpp"

#include <stdexcept>

#include "utils/utils.hpp"

std::string	direction_to_string(Direction dir)
{
	std::string	result;

	switch (dir)
	{
		case Direction::NORTH:
			result = "NORTH";
			break;
		case Direction::EAST:
			result = "EAST";
			break;
		case Direction::SOUTH:
			result = "SOUTH";
			break;
		case Direction::WEST:
			result = "WEST";
			break;
		default:
			throw std::invalid_argument("This error should never happen.");
			break;
	}
	return (result);
}

Direction	string_to_direction(const std::string& str)
{
	std::string	temp = to_upper(str);

	if (temp == "NORTH")
		return Direction::NORTH;
	if (temp == "EAST")
		return Direction::EAST;
	if (temp == "SOUTH")
		return Direction::SOUTH;
	if (temp == "WEST")
		return Direction::WEST;
	return Direction::INVALID;
}

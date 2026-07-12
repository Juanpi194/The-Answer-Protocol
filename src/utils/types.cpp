#include "utils/types.hpp"

#include <stdexcept>

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

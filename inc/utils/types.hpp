#pragma once
#include <string>

enum class Direction
{
	NORTH,
	EAST,
	SOUTH,
	WEST
};

enum class GearType
{
	Iron,
	Fire,
	Ice
};

/**
 * @brief	Casts the provided direction to string.
 * @param	dir	The direction to cast.
 * @returns	The string version of `dir`.
 * @throws	`std::invalid_argument` if unexpected parameter is received.
 */
std::string	direction_to_string(Direction dir);

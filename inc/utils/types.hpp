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

/**
 * @brief	Checks if an instance is a specified class type.
 * @param	ptr	The instance to be evaluated.
 * @returns	`true` if the instance is the class type. `false` otherwise.
 * @example	is_instance<ChestKey>(item) -> `true` if `item` is a `ChestKey`.
 * @note	The body is defined in the .hpp because of the template.
 */
template <typename T, typename C>
bool		is_instance(const C *ptr)
{
	return (dynamic_cast<const T*>(ptr) != nullptr);
}

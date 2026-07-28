#pragma once
#include <string>

enum class Direction
{
	NORTH,
	EAST,
	SOUTH,
	WEST,
	INVALID
};

enum class GearType
{
	IRON,
	FIRE,
	ICE
};

/**
 * @brief	Casts the provided direction to string.
 * @param	dir	The direction to cast.
 * @returns	The string version of `dir`.
 * @throws	`std::invalid_argument` if unexpected parameter is received.
 */
std::string	direction_to_string(Direction dir);

/**
 * @brief	Casts the provided string to Direction. Lowercase strings
 * 			are supported. The provided string will get uppercased.
 * @param	str	The string to cast.
 * @returns	The direction version of `str`. `INVALID` if it does not
 * 			match any direction.
 * @note	`trim_str` is not used in this function.
 */
Direction	string_to_direction(const std::string& str);

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

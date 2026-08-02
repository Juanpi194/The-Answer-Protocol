#include "factories/EnchantmentFactory.hpp"

#include <stdexcept>

#include "utils/utils.hpp"
#include "enchantments/Flame.hpp"
#include "enchantments/Freeze.hpp"

Enchantment	*EnchantmentFactory::create_from_name(const std::string& enchantment_name)
{
	// ? REVIEW: Should trim_str be used?
	std::string	temp = enchantment_name;

	trim_str(temp);
	if (temp == Flame::NAME)
		return (create_flame());
	if (temp == Freeze::NAME)
		return (create_freeze());
	// TODO: Add more enchantments.
	throw std::invalid_argument("No enchantment matches the name '" + enchantment_name + "'.");
}

// Specific enchantments ------------------------------------------------------

Flame	*EnchantmentFactory::create_flame(void) noexcept
{
	return (new Flame());
}

Freeze	*EnchantmentFactory::create_freeze(void) noexcept
{
	return (new Freeze());
}

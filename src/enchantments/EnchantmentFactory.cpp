#include "enchantments/EnchantmentFactory.hpp"

#include <stdexcept>

#include "utils/utils.hpp"
#include "enchantments/Flame.hpp"

Enchantment	*EnchantmentFactory::create_from_name(const std::string& enchantment_name)
{
	// ? REVIEW: Should trim_str be used?
	std::string	temp = enchantment_name;

	trim_str(temp);
	if (temp == Flame::NAME)
		return (create_flame());
	// TODO: Add more enchantments.
	throw std::invalid_argument("No enchantment matches the name '" + enchantment_name + "'.");
}

// Specific enchantments ------------------------------------------------------

Flame	*EnchantmentFactory::create_flame(void) noexcept
{
	return (new Flame());
}

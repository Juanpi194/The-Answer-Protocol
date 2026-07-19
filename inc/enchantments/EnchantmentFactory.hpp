#pragma once
#include <string>

#include "utils/attributes.hpp"

class Enchantment;
class Flame;

class EnchantmentFactory
{
	public:
		static Enchantment	*create_from_name(const std::string& enchantment_name) TAP_RETURNS_NONNULL TAP_UNUSED_RESULT;

		// Specific enchantments ----------------------------------------------

		static Flame		*create_flame(void) noexcept TAP_RETURNS_NONNULL TAP_UNUSED_RESULT;
};

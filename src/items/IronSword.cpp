#include "items/IronSword.hpp"

const std::string	IronSword::NAME = "Iron Sword";
const std::string	IronSword::DESCRIPTION = "Applies extra damage";

void	IronSword::special_use(void) noexcept
{
	// TODO: Logic...
}

// Constructors ---------------------------------------------------------------

IronSword::IronSword(void):
	Weapon(NAME, DESCRIPTION, EXTRA_DAMAGE, GEAR_TYPE)
{
}

IronSword::IronSword(const IronSword& iron_sword):
	Weapon(iron_sword.get_name(), iron_sword.get_description(), iron_sword.get_extra_damage(), iron_sword.get_gear_type())
{
}

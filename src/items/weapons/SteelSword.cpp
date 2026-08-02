#include "items/weapons/SteelSword.hpp"

unsigned int		SteelSword::available_id = 0;
const std::string	SteelSword::PREFIX = "steel_sword.";
const std::string	SteelSword::NAME = "Steel Sword";
const std::string	SteelSword::DESCRIPTION = "Applies extra damage";

// void	SteelSword::special_use(void) noexcept
// {
// 	// TODO: Logic...
// }

// Constructors ---------------------------------------------------------------

SteelSword::SteelSword(void):
	Weapon(Item::PREFIX + PREFIX + std::to_string(available_id++), NAME, DESCRIPTION, EXTRA_DAMAGE, GEAR_TYPE)
{
}

SteelSword::SteelSword(const SteelSword& steel_sword):
	Weapon(Item::PREFIX + PREFIX + std::to_string(available_id++), steel_sword.get_name(), steel_sword.get_description(), steel_sword.get_extra_damage(), steel_sword.get_gear_type())
{
}

SteelSword	*SteelSword::clone(void) const noexcept
{
	return (new SteelSword(*this));
}

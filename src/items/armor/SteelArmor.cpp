#include "items/armor/SteelArmor.hpp"

unsigned int		SteelArmor::available_id = 0;
const std::string	SteelArmor::PREFIX = "steel_armor.";
const std::string	SteelArmor::NAME = "Steel Armor";
const std::string	SteelArmor::DESCRIPTION = "Reduces incoming damage";

// Constructors ---------------------------------------------------------------

SteelArmor::SteelArmor(void):
	Armor(Item::PREFIX + PREFIX + std::to_string(available_id++), NAME, DESCRIPTION, DAMAGE_REDUCTION, GEAR_TYPE)
{
}

SteelArmor::SteelArmor(const SteelArmor& steel_armor):
	Armor(Item::PREFIX + PREFIX + std::to_string(available_id++), steel_armor.get_name(), steel_armor.get_description(), steel_armor.get_damage_reduction(), steel_armor.get_gear_type())
{
}

SteelArmor	*SteelArmor::clone(void) const noexcept
{
	return (new SteelArmor(*this));
}

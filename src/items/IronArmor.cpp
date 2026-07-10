#include "items/IronArmor.hpp"

const std::string	IronArmor::NAME = "Iron Armor";
const std::string	IronArmor::DESCRIPTION = "Reduces incoming damage";

void	IronArmor::special_use(void) noexcept
{
	// TODO: Logic...
}

// Constructors ---------------------------------------------------------------

IronArmor::IronArmor(void):
	Armor(NAME, DESCRIPTION, DAMAGE_REDUCTION, GEAR_TYPE)
{
}

IronArmor::IronArmor(const IronArmor& iron_armor):
	Armor(iron_armor.get_name(), iron_armor.get_description(), iron_armor.get_damage_reduction(), iron_armor.get_gear_type())
{
}

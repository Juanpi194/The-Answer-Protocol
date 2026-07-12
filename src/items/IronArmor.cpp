#include "items/IronArmor.hpp"

unsigned int		IronArmor::available_id = 0;
const std::string	IronArmor::PREFIX = "iron_armor.";
const std::string	IronArmor::NAME = "Iron Armor";
const std::string	IronArmor::DESCRIPTION = "Reduces incoming damage";

void	IronArmor::special_use(void) noexcept
{
	// TODO: Logic...
}

// Constructors ---------------------------------------------------------------

IronArmor::IronArmor(void):
	Armor(Item::PREFIX + PREFIX + std::to_string(available_id++), NAME, DESCRIPTION, DAMAGE_REDUCTION, GEAR_TYPE)
{
}

IronArmor::IronArmor(const IronArmor& iron_armor):
	Armor(Item::PREFIX + PREFIX + std::to_string(available_id++), iron_armor.get_name(), iron_armor.get_description(), iron_armor.get_damage_reduction(), iron_armor.get_gear_type())
{
}

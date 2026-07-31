#include "items/armor/BronzeArmor.hpp"

unsigned int		BronzeArmor::available_id = 0;
const std::string	BronzeArmor::PREFIX = "bronze_armor.";
const std::string	BronzeArmor::NAME = "Bronze Armor";
const std::string	BronzeArmor::DESCRIPTION = "Reduces incoming damage";

// void	BronzeArmor::special_use(void) noexcept
// {
// 	// TODO: Logic...
// }

// Constructors ---------------------------------------------------------------

BronzeArmor::BronzeArmor(void):
	Armor(Item::PREFIX + PREFIX + std::to_string(available_id++), NAME, DESCRIPTION, DAMAGE_REDUCTION, GEAR_TYPE)
{
}

BronzeArmor::BronzeArmor(const BronzeArmor& bronze_armor):
	Armor(Item::PREFIX + PREFIX + std::to_string(available_id++), bronze_armor.get_name(), bronze_armor.get_description(), bronze_armor.get_damage_reduction(), bronze_armor.get_gear_type())
{
}

BronzeArmor	*BronzeArmor::clone(void) const noexcept
{
	return (new BronzeArmor(*this));
}

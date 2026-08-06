#include "items/armor/FrostArmor.hpp"


unsigned int		FrostArmor::available_id = 0;
const std::string	FrostArmor::PREFIX = "frost_armor.";
const std::string	FrostArmor::NAME = "Frost Armor";
const std::string	FrostArmor::DESCRIPTION = "Reduces the incoming damage by a bit, then freezes the attacker.";

// Constructors ---------------------------------------------------------------

FrostArmor::FrostArmor(void):
	Armor(Item::PREFIX + PREFIX + std::to_string(available_id++), NAME, DESCRIPTION, DAMAGE_REDUCTION, GEAR_TYPE)
{
}

FrostArmor::FrostArmor(const FrostArmor& frost_armor):
	Armor(Item::PREFIX + PREFIX + std::to_string(available_id++), frost_armor.get_name(), frost_armor.get_description(), frost_armor.get_damage_reduction(), frost_armor.get_gear_type())
{
}

FrostArmor	*FrostArmor::clone(void) const noexcept
{
	return (new FrostArmor(*this));
}

// Utils ----------------------------------------------------------------------

void	FrostArmor::special_effect(Fighter& user, Fighter& opponent) noexcept
{
	(void)user;
	opponent.apply_status(APPLIED_STATUS);
}

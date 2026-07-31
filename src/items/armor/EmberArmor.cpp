#include "items/armor/EmberArmor.hpp"


unsigned int		EmberArmor::available_id = 0;
const std::string	EmberArmor::PREFIX = "ember_armor.";
const std::string	EmberArmor::NAME = "Ember Armor";
const std::string	EmberArmor::DESCRIPTION = "Reduces the incoming damage by a bit, then burns the attacker.";

// Constructors ---------------------------------------------------------------

EmberArmor::EmberArmor(void):
	Armor(Item::PREFIX + PREFIX + std::to_string(available_id++), NAME, DESCRIPTION, DAMAGE_REDUCTION, GEAR_TYPE)
{
}

EmberArmor::EmberArmor(const EmberArmor& ember_armor):
	Armor(Item::PREFIX + PREFIX + std::to_string(available_id++), ember_armor.get_name(), ember_armor.get_description(), ember_armor.get_damage_reduction(), ember_armor.get_gear_type())
{
}

EmberArmor	*EmberArmor::clone(void) const noexcept
{
	return (new EmberArmor(*this));
}

// Utils ----------------------------------------------------------------------

void	EmberArmor::special_effect(Fighter& user, Fighter& opponent) noexcept
{
	// TODO: Apply burn to opponent
}

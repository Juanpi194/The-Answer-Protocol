#include "items/Armor.hpp"

// Constructors ---------------------------------------------------------------

Armor::Armor(const std::string& name, const std::string& description, const unsigned int damage_reduction, const GearType gear_type):
	Gear(name, description, gear_type),
	damage_reduction(damage_reduction),
	reduced_damage(0)
{
}

Armor::Armor(const Armor& armor):
	Gear(armor.get_name(), armor.get_description(), armor.get_gear_type()),
	damage_reduction(armor.damage_reduction),
	reduced_damage(0)
{
}

Armor::~Armor(void)
{
	// TODO: Free enchantment, ...
}

// Getters and setters --------------------------------------------------------

const unsigned int	Armor::get_damage_reduction(void) const noexcept
{
	return (damage_reduction);
}

unsigned int	Armor::get_reduced_damage(void) const noexcept
{
	return (reduced_damage);
}

// Utils ----------------------------------------------------------------------

void	Armor::use(void) noexcept
{
	// TODO: Logic...
	special_use();
}

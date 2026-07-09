#include "items/Weapon.hpp"

// Constructors ---------------------------------------------------------------

Weapon::Weapon(const std::string& name, const std::string& description, unsigned int extra_damage, const GearType gear_type):
	Gear(name, description, gear_type),
	extra_damage(extra_damage),
	total_extra_damage(0)
{
}

Weapon::Weapon(const Weapon& weapon):
	Gear(weapon.get_name(), weapon.get_description(), weapon.get_gear_type()),
	extra_damage(weapon.extra_damage),
	total_extra_damage(0)
{
}

Weapon::~Weapon(void)
{
	// TODO: Free enchants...
}

// Getters and setters --------------------------------------------------------

unsigned int	Weapon::get_extra_damage(void) const noexcept
{
	return (extra_damage);
}

// Utils ----------------------------------------------------------------------

void	Weapon::use(void) noexcept
{
	// TODO: Logic...
	special_use();
}

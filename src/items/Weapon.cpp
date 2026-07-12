#include "items/Weapon.hpp"

// Constructors ---------------------------------------------------------------

Weapon::Weapon(const std::string& id, const std::string& name, const std::string& description, unsigned int extra_damage, const GearType gear_type):
	Gear(id, name, description, gear_type),
	extra_damage(extra_damage),
	total_extra_damage(0)
{
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

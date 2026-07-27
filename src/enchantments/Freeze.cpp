#include "enchantments/Freeze.hpp"

const std::string			Freeze::NAME = "Freeze";
const std::string			Freeze::DESCRIPTION = "Applies freeze.";
const std::list<GearType>	Freeze::ALLOWED_GEAR_TYPES = {GearType::Iron};

// Constructors ---------------------------------------------------------------

Freeze::Freeze(void):
	Enchantment(NAME, DESCRIPTION, ALLOWED_GEAR_TYPES)
{
}

Freeze::Freeze(const Freeze& freeze):
	Enchantment(freeze.get_name(), freeze.get_description(), freeze.get_allowed_gear_types())
{
}

Freeze	*Freeze::clone(void) const noexcept
{
	return (new Freeze(*this));
}

// Utils ----------------------------------------------------------------------

void	Freeze::effect(Fighter& user, Fighter& affected) const noexcept
{
	// TODO: Logic...
}

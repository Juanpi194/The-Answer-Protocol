#include "enchantments/Flame.hpp"

const std::string			Flame::NAME = "Flame";
const std::string			Flame::DESCRIPTION = "Applies burn.";
const std::list<GearType>	Flame::ALLOWED_GEAR_TYPES = {GearType::Iron};

// Constructors ---------------------------------------------------------------

Flame::Flame(void):
	Enchantment(NAME, DESCRIPTION, ALLOWED_GEAR_TYPES)
{
}

Flame::Flame(const Flame& flame):
	Enchantment(flame.get_name(), flame.get_description(), flame.get_allowed_gear_types())
{
}

void	Flame::effect(void) noexcept
{
	// TODO: Logic...
}

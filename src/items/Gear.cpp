#include "items/Gear.hpp"

#include "utils/utils.hpp"

// Constructors ---------------------------------------------------------------

Gear::Gear(const std::string& name, const std::string& description, GearType gear_type):
	Item(name, description),
	gear_type(gear_type),
	enchantment(nullptr)
{
}

Gear::Gear(const Gear& gear):
	Item(gear.get_name(), gear.get_description()),
	gear_type(gear.gear_type),
	enchantment(nullptr)
{
	// ? REVIEW: Does the copy have the same enchantments the one being copied has?
}

Gear::~Gear(void)
{
	// TODO: Free enchantments...
}

// Getters and setters --------------------------------------------------------

GearType	Gear::get_gear_type(void) const noexcept
{
	return (gear_type);
}

Enchantment	*Gear::get_enchantment(void) const noexcept
{
	if (!enchantment)
		log("Gear '" + get_name() + "' has no enchantment.", LogLevel::INFO);
	return (enchantment);
}

void	Gear::set_enchantment(Enchantment *enchantment)
{
	if (!enchantment)
		log("No enchantment provided to set to '" + get_name() + "'.", LogLevel::INFO);
	// TODO: Logic of checking if can be set...
	this->enchantment = enchantment;
}

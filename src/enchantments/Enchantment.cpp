#include "enchantments/Enchantment.hpp"

#include <stdexcept>

#include "items/Gear.hpp"
#include "utils/utils.hpp"

bool	Enchantment::validate_arguments(const std::string& name, const std::string& description)
{
	// TODO: Logic...
	return (true);
}

bool	Enchantment::can_apply(Gear& gear)
{
	// ? REVIEW: Maybe there is a more efficient way to check
	for (GearType gear_type: allowed_gear_types)
	{
		if (gear.get_gear_type() == gear_type)
			return (true);
	}
	return (false);
}

// Constructors ---------------------------------------------------------------

Enchantment::Enchantment(const std::string& name, const std::string& description, const std::list<GearType> allowed_gear_types):
	name(name),
	description(description),
	allowed_gear_types(allowed_gear_types)
{
	if (!validate_arguments(name, description))
		throw std::invalid_argument("Provided arguments for the enchantment are invalid. Enchantment initialization failed.");
}

Enchantment::Enchantment(const Enchantment& enchantment):
	name(enchantment.name),
	description(enchantment.description),
	allowed_gear_types(enchantment.allowed_gear_types)
{
}

// Getters and setters --------------------------------------------------------

std::string					Enchantment::get_name(void) const noexcept
{
	return (name);
}

std::string					Enchantment::get_description(void) const noexcept
{
	return (description);
}

const std::list<GearType>	Enchantment::get_allowed_gear_types(void) const noexcept
{
	return (allowed_gear_types);
}

// Utils ----------------------------------------------------------------------

void	Enchantment::apply_to(Gear& gear)
{
	// ? REVIEW: Does this make sense?
	if (can_apply(gear))
		gear.set_enchantment(this);
	else
		log("Tried to enchant '" + gear.get_name() + "' with '" + name + "', but failed", LogLevel::WARNING);
}

void	Enchantment::effect(void) noexcept
{
	// TODO: Logic ...
}

#include "items/Gear.hpp"

#include "utils/utils.hpp"
#include "enchantments/Enchantment.hpp"

// Constructors ---------------------------------------------------------------

Gear::Gear(const std::string& id, const std::string& name, const std::string& description, GearType gear_type):
	Item(id, name, description),
	gear_type(gear_type),
	enchantment(nullptr)
{
}

Gear::~Gear(void)
{
	if (enchantment)
		delete (enchantment);
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
	// ? REVIEW: Should this method check if the enchantment can be set?
	// ?		 Or should it be a quick and dangerous way to set an enchantment?
	this->enchantment = enchantment;
}

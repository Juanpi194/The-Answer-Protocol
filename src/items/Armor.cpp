#include "items/Armor.hpp"

// Constructors ---------------------------------------------------------------

Armor::Armor(const std::string& id, const std::string& name, const std::string& description, unsigned int reduced_damage):
	Gear(id, name, description),
	reduced_damage(reduced_damage)
{
}

// Getters and setters --------------------------------------------------------

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

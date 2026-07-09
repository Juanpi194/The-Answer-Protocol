#include "items/Consumable.hpp"

// Constructors ---------------------------------------------------------------

Consumable::Consumable(const std::string& name, const std::string& description):
	Item(name, description)
{
}

Consumable::Consumable(const Consumable& consumable):
	Item(consumable.get_name(), consumable.get_description())
{
}

// Utils ----------------------------------------------------------------------

void	Consumable::use(void) noexcept
{
	// TODO: Logic...
}

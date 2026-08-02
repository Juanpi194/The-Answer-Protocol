#include "items/consumables/IcePotion.hpp"


unsigned int		IcePotion::available_id = 0;
const std::string	IcePotion::PREFIX = "ice_potion.";
const std::string	IcePotion::NAME = "Ice Potion";
const std::string	IcePotion::DESCRIPTION = "Grants the user an icy boost when consumed.";

// Constructors ---------------------------------------------------------------

IcePotion::IcePotion(void):
	Consumable(Item::PREFIX + PREFIX + std::to_string(available_id++), NAME, DESCRIPTION)
{
}

IcePotion::IcePotion(const IcePotion& ice_potion):
	Consumable(Item::PREFIX + PREFIX + std::to_string(available_id++), ice_potion.get_name(), ice_potion.get_description())
{
}

IcePotion	*IcePotion::clone(void) const noexcept
{
	return (new IcePotion(*this));
}

// Utils ----------------------------------------------------------------------

void	IcePotion::consume(Fighter& user) noexcept
{
	// TODO: Add ice effect logic...
}

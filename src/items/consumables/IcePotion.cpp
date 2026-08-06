#include "items/consumables/IcePotion.hpp"

#include "characters/Fighter.hpp"


unsigned int		IcePotion::available_id = 0;
const std::string	IcePotion::PREFIX = "ice_potion.";
const std::string	IcePotion::NAME = "Ice Potion";
const std::string	IcePotion::DESCRIPTION = "Restores the user's status if it is frozen.";

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
	if (user.get_status() == Status::FROZEN)
		user.set_status(Status::HEALTHY);
}

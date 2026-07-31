#include "items/consumables/FirePotion.hpp"


unsigned int		FirePotion::available_id = 0;
const std::string	FirePotion::PREFIX = "fire_potion.";
const std::string	FirePotion::NAME = "Fire Potion";
const std::string	FirePotion::DESCRIPTION = "Grants the user a fiery boost when consumed.";

// Constructors ---------------------------------------------------------------

FirePotion::FirePotion(void):
	Consumable(Item::PREFIX + PREFIX + std::to_string(available_id++), NAME, DESCRIPTION)
{
}

FirePotion::FirePotion(const FirePotion& fire_potion):
	Consumable(Item::PREFIX + PREFIX + std::to_string(available_id++), fire_potion.get_name(), fire_potion.get_description())
{
}

FirePotion	*FirePotion::clone(void) const noexcept
{
	return (new FirePotion(*this));
}

// Utils ----------------------------------------------------------------------

void	FirePotion::consume(Fighter& user) noexcept
{
	// TODO: Add fire effect logic...
}

#include "items/HealingPotion.hpp"

unsigned int		HealingPotion::available_id = 0;
const std::string	HealingPotion::PREFIX = "healing_potion.";
const std::string	HealingPotion::NAME = "Healing Potion";
const std::string	HealingPotion::DESCRIPTION = "Heals all user's hp.";

// Constructors ---------------------------------------------------------------

HealingPotion::HealingPotion(void):
	Consumable(Item::PREFIX + PREFIX + std::to_string(available_id++), NAME, DESCRIPTION)
{
}

HealingPotion::HealingPotion(const HealingPotion& healing_potion):
	Consumable(Item::PREFIX + PREFIX + std::to_string(available_id++), healing_potion.get_name(), healing_potion.get_description())
{
}

HealingPotion	*HealingPotion::clone(void) const noexcept
{
	return (new HealingPotion(*this));
}

// Utils ----------------------------------------------------------------------

void	HealingPotion::consume(Fighter& user) noexcept
{
	// TODO: Add heal and restore strength stat...
}

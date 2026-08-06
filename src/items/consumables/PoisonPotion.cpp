#include "items/consumables/PoisonPotion.hpp"

#include "characters/Fighter.hpp"


unsigned int		PoisonPotion::available_id = 0;
const std::string	PoisonPotion::PREFIX = "poison_potion.";
const std::string	PoisonPotion::NAME = "Poison Potion";
const std::string	PoisonPotion::DESCRIPTION = "Restores the user's status if it is poisoned.";

// Constructors ---------------------------------------------------------------

PoisonPotion::PoisonPotion(void):
	Consumable(Item::PREFIX + PREFIX + std::to_string(available_id++), NAME, DESCRIPTION)
{
}

PoisonPotion::PoisonPotion(const PoisonPotion& poison_potion):
	Consumable(Item::PREFIX + PREFIX + std::to_string(available_id++), poison_potion.get_name(), poison_potion.get_description())
{
}

PoisonPotion	*PoisonPotion::clone(void) const noexcept
{
	return (new PoisonPotion(*this));
}

// Utils ----------------------------------------------------------------------

void	PoisonPotion::consume(Fighter& user) noexcept
{
	if (user.get_status() == Status::POISONED)
		user.set_status(Status::HEALTHY);
}

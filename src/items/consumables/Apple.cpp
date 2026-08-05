#include "items/consumables/Apple.hpp"

#include "characters/Fighter.hpp"

unsigned int		Apple::available_id = 0;
const std::string	Apple::PREFIX = "apple.";
const std::string	Apple::NAME = "Apple";
const std::string	Apple::DESCRIPTION = "Heals for a bit, and restores the strength of the user if it was lowered.";

// Constructors ---------------------------------------------------------------

Apple::Apple(void):
	Consumable(Item::PREFIX + PREFIX + std::to_string(available_id++), NAME, DESCRIPTION)
{
}

Apple::Apple(const Apple& apple):
	Consumable(Item::PREFIX + PREFIX + std::to_string(available_id++), apple.get_name(), apple.get_description())
{
}

Apple	*Apple::clone(void) const noexcept
{
	return (new Apple(*this));
}

// Utils ----------------------------------------------------------------------

void	Apple::consume(Fighter& user) noexcept
{
	user.heal(HEALING_POINTS);
	if (user.get_stats().current_strength < user.get_stats().strength)
    	user.restore_stat(Stat::STRENGTH);
}

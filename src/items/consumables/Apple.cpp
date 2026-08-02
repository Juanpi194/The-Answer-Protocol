#include "items/consumables/Apple.hpp"


unsigned int		Apple::available_id = 0;
const std::string	Apple::PREFIX = "apple.";
const std::string	Apple::NAME = "Apple";
const std::string	Apple::DESCRIPTION = "Heals for a bit, and restores the strength of the user.";

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
	// TODO: Add heal and restore strength stat...
}

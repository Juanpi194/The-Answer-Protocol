#include "items/Shield.hpp"

// Constructors ---------------------------------------------------------------

Shield::Shield(const std::string& name, const std::string& description, const unsigned int max_uses):
	Item(name, description),
	uses(0),
	max_uses(max_uses)
{
}

Shield::Shield(const Shield& shield):
	Item(shield.get_name(), shield.get_description()),
	uses(0),
	max_uses(shield.max_uses)
{
}

// Getters and setters --------------------------------------------------------

unsigned int		Shield::get_uses(void) const noexcept
{
	return (uses);
}

const unsigned int	Shield::get_max_uses(void) const noexcept
{
	return (max_uses);
}

void	Shield::set_uses(unsigned int uses) noexcept
{
	this->uses = uses;
}

// Utils ----------------------------------------------------------------------

void	Shield::protect(void) noexcept
{
	// TODO: Logic...
	uses--;
}

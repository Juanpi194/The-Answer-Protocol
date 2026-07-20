#include "items/Shield.hpp"

// Constructors ---------------------------------------------------------------

Shield::Shield(const std::string& id, const std::string& name, const std::string& description, const unsigned int max_uses):
	Item(id, name, description),
	uses(0),
	max_uses(max_uses)
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

// void	Shield::use(void) noexcept
// {
// 	// TODO: Logic...
// }

void	Shield::protect(Fighter& user, Fighter& attacker) noexcept
{
	// TODO: Logic...
	uses--;
}

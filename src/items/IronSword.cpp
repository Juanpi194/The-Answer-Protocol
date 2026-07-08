#include "items/IronSword.hpp"

const std::string	IronSword::NAME = "Iron Sword";
const std::string	IronSword::DESCRIPTION = "Applies extra damage";

void	IronSword::special_use(void) noexcept
{
	// TODO: Logic...
}

// Constructors ---------------------------------------------------------------

IronSword::IronSword(const std::string& id):
	Weapon(id, NAME, DESCRIPTION, EXTRA_DAMAGE, GEAR_TYPE)
{
}

IronSword::~IronSword(void)
{
	// TODO: Free enchantment...
}

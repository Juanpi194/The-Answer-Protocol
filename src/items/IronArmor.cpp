#include "items/IronArmor.hpp"

const std::string	IronArmor::NAME = "Iron Armor";
const std::string	IronArmor::DESCRIPTION = "Reduces incoming damage";

void	IronArmor::special_use(void) noexcept
{
	// TODO: Logic...
}

// Constructors ---------------------------------------------------------------

IronArmor::IronArmor(const std::string& id):
	Armor(id, NAME, DESCRIPTION, DAMAGE_REDUCTION, GEAR_TYPE)
{
}

IronArmor::~IronArmor(void)
{
	// TODO: Free enchantment...
}

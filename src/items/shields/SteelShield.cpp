#include "items/shields/SteelShield.hpp"

unsigned int		SteelShield::available_id = 0;
const std::string	SteelShield::PREFIX = "steel_shield.";
const std::string	SteelShield::NAME = "Steel Shield";
const std::string	SteelShield::DESCRIPTION = "Blocks attacks.";

// Constructors ---------------------------------------------------------------

SteelShield::SteelShield(void):
	Shield(Item::PREFIX + PREFIX + std::to_string(available_id++), NAME, DESCRIPTION, MAX_USES)
{
}

SteelShield::SteelShield(const SteelShield& steel_shield):
	Shield(Item::PREFIX + PREFIX + std::to_string(available_id++), steel_shield.get_name(), steel_shield.get_description(), steel_shield.get_max_uses())
{
}

SteelShield	*SteelShield::clone(void) const noexcept
{
	return (new SteelShield(*this));
}

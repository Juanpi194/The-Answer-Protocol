#include "items/IronShield.hpp"

const std::string	IronShield::NAME = "Iron Shield";
const std::string	IronShield::DESCRIPTION = "Blocks attacks.";

// Constructors ---------------------------------------------------------------

IronShield::IronShield(void):
	Shield(NAME, DESCRIPTION, MAX_USES)
{
}

IronShield::IronShield(const IronShield& iron_shield):
	Shield(iron_shield.get_name(), iron_shield.get_description(), iron_shield.get_max_uses())
{
}

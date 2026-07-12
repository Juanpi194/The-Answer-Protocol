#include "items/IronShield.hpp"

unsigned int		IronShield::available_id = 0;
const std::string	IronShield::PREFIX = "iron_shield.";
const std::string	IronShield::NAME = "Iron Shield";
const std::string	IronShield::DESCRIPTION = "Blocks attacks.";

// Constructors ---------------------------------------------------------------

IronShield::IronShield(void):
	Shield(Item::PREFIX + PREFIX + std::to_string(available_id++), NAME, DESCRIPTION, MAX_USES)
{
}

IronShield::IronShield(const IronShield& iron_shield):
	Shield(Item::PREFIX + PREFIX + std::to_string(available_id++), iron_shield.get_name(), iron_shield.get_description(), iron_shield.get_max_uses())
{
}

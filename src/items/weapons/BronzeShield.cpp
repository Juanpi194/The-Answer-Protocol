#include "items/weapons/BronzeShield.hpp"

unsigned int		BronzeShield::available_id = 0;
const std::string	BronzeShield::PREFIX = "bronze_shield.";
const std::string	BronzeShield::NAME = "Bronze Shield";
const std::string	BronzeShield::DESCRIPTION = "Blocks attacks.";

// Constructors ---------------------------------------------------------------

BronzeShield::BronzeShield(void):
	Shield(Item::PREFIX + PREFIX + std::to_string(available_id++), NAME, DESCRIPTION, MAX_USES)
{
}

BronzeShield::BronzeShield(const BronzeShield& bronze_shield):
	Shield(Item::PREFIX + PREFIX + std::to_string(available_id++), bronze_shield.get_name(), bronze_shield.get_description(), bronze_shield.get_max_uses())
{
}

BronzeShield	*BronzeShield::clone(void) const noexcept
{
	return (new BronzeShield(*this));
}

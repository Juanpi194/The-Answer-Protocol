#include "items/weapons/BronzeSword.hpp"

unsigned int		BronzeSword::available_id = 0;
const std::string	BronzeSword::PREFIX = "bronze_sword.";
const std::string	BronzeSword::NAME = "Bronze Sword";
const std::string	BronzeSword::DESCRIPTION = "Applies extra damage";

// void	BronzeSword::special_use(void) noexcept
// {
// 	// TODO: Logic...
// }

// Constructors ---------------------------------------------------------------

BronzeSword::BronzeSword(void):
	Weapon(Item::PREFIX + PREFIX + std::to_string(available_id++), NAME, DESCRIPTION, EXTRA_DAMAGE, GEAR_TYPE)
{
}

BronzeSword::BronzeSword(const BronzeSword& bronze_sword):
	Weapon(Item::PREFIX + PREFIX + std::to_string(available_id++), bronze_sword.get_name(), bronze_sword.get_description(), bronze_sword.get_extra_damage(), bronze_sword.get_gear_type())
{
}

BronzeSword	*BronzeSword::clone(void) const noexcept
{
	return (new BronzeSword(*this));
}

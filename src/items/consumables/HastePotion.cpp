#include "items/consumables/HastePotion.hpp"

#include "characters/Fighter.hpp"


unsigned int		HastePotion::available_id = 0;
const std::string	HastePotion::PREFIX = "haste_potion.";
const std::string	HastePotion::NAME = "Haste Potion";
const std::string	HastePotion::DESCRIPTION = "Grants the user a burst of speed when consumed.";

// Constructors ---------------------------------------------------------------

HastePotion::HastePotion(void):
	Consumable(Item::PREFIX + PREFIX + std::to_string(available_id++), NAME, DESCRIPTION)
{
}

HastePotion::HastePotion(const HastePotion& haste_potion):
	Consumable(Item::PREFIX + PREFIX + std::to_string(available_id++), haste_potion.get_name(), haste_potion.get_description())
{
}

HastePotion	*HastePotion::clone(void) const noexcept
{
	return (new HastePotion(*this));
}

// Utils ----------------------------------------------------------------------

void	HastePotion::consume(Fighter& user) noexcept
{
	user.change_stat(Stat::SPEED, HASTE_POINTS);
}

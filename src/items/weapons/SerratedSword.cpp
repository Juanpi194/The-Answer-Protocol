#include "items/weapons/SerratedSword.hpp"

unsigned int		SerratedSword::available_id = 0;
const std::string	SerratedSword::PREFIX = "serrated_sword.";
const std::string	SerratedSword::NAME = "Serrated Sword";
const std::string	SerratedSword::DESCRIPTION = "Applies extra damage, and poisons the target.";

// Constructors ---------------------------------------------------------------

SerratedSword::SerratedSword(void):
	Weapon(Item::PREFIX + PREFIX + std::to_string(available_id++), NAME, DESCRIPTION, EXTRA_DAMAGE, GEAR_TYPE)
{
}

SerratedSword::SerratedSword(const SerratedSword& serrated_sword):
	Weapon(Item::PREFIX + PREFIX + std::to_string(available_id++), serrated_sword.get_name(), serrated_sword.get_description(), serrated_sword.get_extra_damage(), serrated_sword.get_gear_type())
{
}

SerratedSword	*SerratedSword::clone(void) const noexcept
{
	return (new SerratedSword(*this));
}

// Utils ----------------------------------------------------------------------

void	SerratedSword::special_effect(Fighter& user, Fighter& opponent) noexcept
{
	opponent.apply_status(APPLIED_STATUS);
}

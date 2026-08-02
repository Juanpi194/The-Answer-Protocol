#include "items/weapons/FrostSword.hpp"

unsigned int		FrostSword::available_id = 0;
const std::string	FrostSword::PREFIX = "frost_sword.";
const std::string	FrostSword::NAME = "Frost Sword";
const std::string	FrostSword::DESCRIPTION = "Applies extra damage, and applies freeze to the target.";

// Constructors ---------------------------------------------------------------

FrostSword::FrostSword(void):
	Weapon(Item::PREFIX + PREFIX + std::to_string(available_id++), NAME, DESCRIPTION, EXTRA_DAMAGE, GEAR_TYPE)
{
}

FrostSword::FrostSword(const FrostSword& frost_sword):
	Weapon(Item::PREFIX + PREFIX + std::to_string(available_id++), frost_sword.get_name(), frost_sword.get_description(), frost_sword.get_extra_damage(), frost_sword.get_gear_type())
{
}

FrostSword	*FrostSword::clone(void) const noexcept
{
	return (new FrostSword(*this));
}

// Utils ----------------------------------------------------------------------

void	FrostSword::special_effect(Fighter& user, Fighter& opponent) noexcept
{
	// TODO: Apply freeze to opponent, ...
}

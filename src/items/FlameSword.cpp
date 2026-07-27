#include "items/FlameSword.hpp"

unsigned int		FlameSword::available_id = 0;
const std::string	FlameSword::PREFIX = "flame_sword.";
const std::string	FlameSword::NAME = "Flame Sword";
const std::string	FlameSword::DESCRIPTION = "Applies extra damage, and applies burn to the target.";

// Constructors ---------------------------------------------------------------

FlameSword::FlameSword(void):
	Weapon(Item::PREFIX + PREFIX + std::to_string(available_id++), NAME, DESCRIPTION, EXTRA_DAMAGE, GEAR_TYPE)
{
}

FlameSword::FlameSword(const FlameSword& flame_sword):
	Weapon(Item::PREFIX + PREFIX + std::to_string(available_id++), flame_sword.get_name(), flame_sword.get_description(), flame_sword.get_extra_damage(), flame_sword.get_gear_type())
{
}

FlameSword	*FlameSword::clone(void) const noexcept
{
	return (new FlameSword(*this));
}

// Utils ----------------------------------------------------------------------

void	FlameSword::special_effect(Fighter& user, Fighter& opponent) noexcept
{
	// TODO: Apply burn to opponent, ...
}

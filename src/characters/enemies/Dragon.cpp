#include "characters/enemies/Dragon.hpp"

unsigned int		Dragon::available_id = 0;
const std::string	Dragon::PREFIX = "dragon.";
const std::string	Dragon::NAME = "Dragon";
const std::string	Dragon::DESCRIPTION = "The fearsome final guardian.";
const t_stats		Dragon::DEFAULT_STATS = {6, 60, 12, 6, 3, 60, 12, 6, 3};

// Constructors ---------------------------------------------------------------

Dragon::Dragon(void):
	Character(NAME),
	Enemy(NPC::PREFIX + PREFIX + std::to_string(available_id++), NAME, DESCRIPTION, DEFAULT_STATS, DEFAULT_GOLD)
{
	// TODO: Maybe add a default armor and weapon?
}

Dragon::Dragon(const Dragon& dragon):
	Character(NAME),
	Enemy(NPC::PREFIX + PREFIX + std::to_string(available_id++), NAME, DESCRIPTION, DEFAULT_STATS, DEFAULT_GOLD)
{
}

Dragon	*Dragon::clone(void) const noexcept
{
	return (new Dragon());
}

// Utils ----------------------------------------------------------------------

FightChoice	Dragon::choose_action(void) const noexcept
{
	// TODO: Logic...

	// Dumb example
	FightChoice	choice = {FightAction::ATTACK};
	return (choice);
}

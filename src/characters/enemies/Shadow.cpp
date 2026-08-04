#include "characters/enemies/Shadow.hpp"

unsigned int		Shadow::available_id = 0;
const std::string	Shadow::PREFIX = "shadow.";
const std::string	Shadow::NAME = "Shadow";
const std::string	Shadow::DESCRIPTION = "A far more powerful shadow.";
const t_stats		Shadow::DEFAULT_STATS = {4, 30, 7, 4, 3, 30, 7, 4, 3};

// Constructors ---------------------------------------------------------------

Shadow::Shadow(void):
	Character(NAME),
	Enemy(NPC::PREFIX + PREFIX + std::to_string(available_id++), NAME, DESCRIPTION, DEFAULT_STATS, DEFAULT_GOLD)
{
	// TODO: Maybe add a default armor and weapon?
}

Shadow::Shadow(const Shadow& shadow):
	Character(NAME),
	Enemy(NPC::PREFIX + PREFIX + std::to_string(available_id++), NAME, DESCRIPTION, DEFAULT_STATS, DEFAULT_GOLD)
{
}

Shadow	*Shadow::clone(void) const noexcept
{
	return (new Shadow());
}

// Utils ----------------------------------------------------------------------

FightChoice	Shadow::choose_action(void) const noexcept
{
	// TODO: Logic...

	// Dumb example
	FightChoice	choice = {FightAction::ATTACK};
	return (choice);
}

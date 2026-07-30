#include "characters/enemies/Goblin.hpp"

unsigned int		Goblin::available_id = 0;
const std::string	Goblin::PREFIX = "goblin.";
const std::string	Goblin::NAME = "Goblin";
const std::string	Goblin::DESCRIPTION = "Es muy tonto";
const t_stats		Goblin::DEFAULT_STATS = {3, 10, 5, 3, 2};

// Constructors ---------------------------------------------------------------

Goblin::Goblin(void):
	Character(NAME),
	Enemy(NPC::PREFIX + PREFIX + std::to_string(available_id++), NAME, DESCRIPTION, DEFAULT_STATS, DEFAULT_GOLD)
{
	// TODO: Maybe add a default armor and weapon?
}

Goblin::Goblin(const Goblin& goblin):
	Character(NAME),
	Enemy(NPC::PREFIX + PREFIX + std::to_string(available_id++), NAME, DESCRIPTION, DEFAULT_STATS, DEFAULT_GOLD)
{
}

Goblin	*Goblin::clone(void) const noexcept
{
	return (new Goblin());
}

// Utils ----------------------------------------------------------------------

FightChoice	Goblin::choose_action(void) const noexcept
{
	// TODO: Logic...

	// Dumb example
	FightChoice	choice = {FightAction::ATTACK};
	return (choice);
}

#include "characters/enemies/Wizard.hpp"

unsigned int		Wizard::available_id = 0;
const std::string	Wizard::PREFIX = "wizard.";
const std::string	Wizard::NAME = "Wizard";
const std::string	Wizard::DESCRIPTION = "A skilled elemental spellcaster.";
const t_stats		Wizard::DEFAULT_STATS = {3, 20, 5, 3, 3, 20, 5, 3, 3};

// Constructors ---------------------------------------------------------------

Wizard::Wizard(void):
	Character(NAME),
	Enemy(NPC::PREFIX + PREFIX + std::to_string(available_id++), NAME, DESCRIPTION, DEFAULT_STATS, DEFAULT_GOLD)
{
	// TODO: Maybe add a default armor and weapon?
}

Wizard::Wizard(const Wizard& wizard):
	Character(NAME),
	Enemy(NPC::PREFIX + PREFIX + std::to_string(available_id++), NAME, DESCRIPTION, DEFAULT_STATS, DEFAULT_GOLD)
{
}

Wizard	*Wizard::clone(void) const noexcept
{
	return (new Wizard());
}

// Utils ----------------------------------------------------------------------

FightChoice	Wizard::choose_action(void) const noexcept
{
	// TODO: Logic...

	// Dumb example
	FightChoice	choice = {FightAction::ATTACK};
	return (choice);
}

#include "characters/enemies/Ogre.hpp"

unsigned int		Ogre::available_id = 0;
const std::string	Ogre::PREFIX = "ogre.";
const std::string	Ogre::NAME = "Ogre";
const std::string	Ogre::DESCRIPTION = "A huge and brutal beast.";
const t_stats		Ogre::DEFAULT_STATS = {2, 15, 4, 3, 1, 15, 4, 3, 1};

// Constructors ---------------------------------------------------------------

Ogre::Ogre(void):
	Character(NAME),
	Enemy(NPC::PREFIX + PREFIX + std::to_string(available_id++), NAME, DESCRIPTION, DEFAULT_STATS, DEFAULT_GOLD)
{
	// TODO: Maybe add a default armor and weapon?
}

Ogre::Ogre(const Ogre& ogre):
	Character(NAME),
	Enemy(NPC::PREFIX + PREFIX + std::to_string(available_id++), NAME, DESCRIPTION, DEFAULT_STATS, DEFAULT_GOLD)
{
}

Ogre	*Ogre::clone(void) const noexcept
{
	return (new Ogre());
}

// Utils ----------------------------------------------------------------------

FightChoice	Ogre::choose_action(void) const noexcept
{
	// TODO: Logic...

	// Dumb example
	FightChoice	choice = {FightAction::ATTACK};
	return (choice);
}

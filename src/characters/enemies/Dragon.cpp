#include "characters/enemies/Dragon.hpp"

#include "factories/ItemFactory.hpp"
#include "items/armor/EmberArmor.hpp"

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
	set_armor(ItemFactory::create_ember_armor());
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
	int	roll;

	roll = rand() % 3;
	if (roll == 0)
		return (FightChoice{FightAction::DEFEND});
	return (FightChoice{FightAction::ATTACK});
}

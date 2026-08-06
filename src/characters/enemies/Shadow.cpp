#include "characters/enemies/Shadow.hpp"

#include "factories/ItemFactory.hpp"
#include "items/armor/SpikeArmor.hpp"
#include "items/shields/IronShield.hpp"

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
	set_armor(ItemFactory::create_spike_armor());
	set_shield(ItemFactory::create_iron_shield());
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
	int	roll;

	roll = rand() % 3;
	if (roll == 0)
		return (FightChoice{FightAction::DEFEND});
	return (FightChoice{FightAction::ATTACK});
}

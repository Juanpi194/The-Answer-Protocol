#include "characters/enemies/Shadow.hpp"

#include "factories/ItemFactory.hpp"
#include "items/armor/SpikeArmor.hpp"
#include "items/shields/IronShield.hpp"
#include "items/consumables/HealingPotion.hpp"

unsigned int		Shadow::available_id = 0;
const std::string	Shadow::PREFIX = "shadow.";
const std::string	Shadow::NAME = "Shadow";
const std::string	Shadow::DESCRIPTION = "A far more powerful shadow.";
const t_stats		Shadow::DEFAULT_STATS = {4, 15, 5, 3, 4, 15, 5, 3, 4};

// Constructors ---------------------------------------------------------------

Shadow::Shadow(void):
	Character(NAME),
	Enemy(NPC::PREFIX + PREFIX + std::to_string(available_id++), NAME, DESCRIPTION, DEFAULT_STATS, DEFAULT_GOLD)
{
	set_armor(ItemFactory::create_spike_armor());
	set_shield(ItemFactory::create_iron_shield());
	get_inventory().add_item(ItemFactory::create_healing_potion());
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

FightChoice	Shadow::choose_action(void) noexcept
{
	int			roll;
	Consumable	*consumable;

	roll = rand() % 3;
	consumable = roll_consumable(CONSUME_CHANCE);
	if (consumable)
		return (FightChoice{FightAction::CONSUME, consumable});
	if (roll == 0)
		return (FightChoice{FightAction::DEFEND});
	return (FightChoice{FightAction::ATTACK});
}

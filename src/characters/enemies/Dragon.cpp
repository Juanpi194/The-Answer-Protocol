#include "characters/enemies/Dragon.hpp"

#include "factories/ItemFactory.hpp"
#include "items/armor/EmberArmor.hpp"
#include "items/consumables/HastePotion.hpp"
#include "items/consumables/HealingPotion.hpp"

unsigned int		Dragon::available_id = 0;
const std::string	Dragon::PREFIX = "dragon.";
const std::string	Dragon::NAME = "Dragon";
const std::string	Dragon::DESCRIPTION = "The fearsome final guardian.";
const t_stats		Dragon::DEFAULT_STATS = {6, 26, 6, 5, 4, 26, 6, 5, 4};

// Constructors ---------------------------------------------------------------

Dragon::Dragon(void):
	Character(NAME),
	Enemy(NPC::PREFIX + PREFIX + std::to_string(available_id++), NAME, DESCRIPTION, DEFAULT_STATS, DEFAULT_GOLD)
{
	set_armor(ItemFactory::create_ember_armor());
	get_inventory().add_item(ItemFactory::create_haste_potion());
	get_inventory().add_item(ItemFactory::create_healing_potion());
	get_inventory().add_item(ItemFactory::create_healing_potion());
}

Dragon::Dragon(const Dragon& dragon):
	Character(NAME),
	Enemy(NPC::PREFIX + PREFIX + std::to_string(available_id++), NAME, DESCRIPTION, DEFAULT_STATS, DEFAULT_GOLD)
{
	(void)dragon;
}

Dragon	*Dragon::clone(void) const noexcept
{
	return (new Dragon());
}

// Utils ----------------------------------------------------------------------

FightChoice	Dragon::choose_action(void) noexcept
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

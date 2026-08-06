#include "characters/enemies/Goblin.hpp"

#include "factories/ItemFactory.hpp"
#include "items/weapons/SteelSword.hpp"
#include "items/consumables/Apple.hpp"

unsigned int		Goblin::available_id = 0;
const std::string	Goblin::PREFIX = "goblin.";
const std::string	Goblin::NAME = "Goblin";
const std::string	Goblin::DESCRIPTION = "A small, foolish creature.";
const t_stats		Goblin::DEFAULT_STATS = {1, 7, 2, 2, 1, 7, 2, 2, 1};

// Constructors ---------------------------------------------------------------

Goblin::Goblin(void):
	Character(NAME),
	Enemy(NPC::PREFIX + PREFIX + std::to_string(available_id++), NAME, DESCRIPTION, DEFAULT_STATS, DEFAULT_GOLD)
{
	set_weapon(ItemFactory::create_steel_sword());
	get_inventory().add_item(ItemFactory::create_apple());
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

FightChoice	Goblin::choose_action(void) noexcept
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

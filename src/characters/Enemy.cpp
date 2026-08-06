#include "characters/Enemy.hpp"

#include "items/Consumable.hpp"

Consumable	*Enemy::roll_consumable(unsigned int chance) noexcept
{
	if (((unsigned int)(rand() % 100)) < chance)
		return (get_inventory().find_item<Consumable>());
	return (nullptr);
}

// Constructors ---------------------------------------------------------------

Enemy::Enemy(const std::string& id, const std::string& name, const std::string& description, t_stats stats, unsigned int gold):
	Character(name),
	NPC(id, name, description),
	Fighter(name, stats),
	gold(gold)
{
}

// Getters and setters --------------------------------------------------------

unsigned int	Enemy::get_gold(void) const noexcept
{
	return (gold);
}

// Utils ----------------------------------------------------------------------

const std::string	Enemy::on_talk(Player& player) noexcept
{
	(void)player;
	return (get_name() + " is focused...");
}

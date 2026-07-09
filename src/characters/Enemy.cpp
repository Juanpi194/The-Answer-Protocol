#include "characters/Enemy.hpp"

// Constructors ---------------------------------------------------------------

Enemy::Enemy(const std::string& name, const std::string& description, t_stats stats, unsigned int gold):
	Character(name),
	NPC(name, description),
	Fighter(name, stats),
	gold(gold)
{
}

Enemy::Enemy(const Enemy& enemy):
	Character(enemy.get_name()),
	NPC(enemy.get_name(), enemy.get_description()),
	Fighter(enemy.get_name(), enemy.stats),
	gold(enemy.gold)
{
}

Enemy::~Enemy(void)
{
	// TODO: Free weapon and armor if has one...
}

// Getters and setters --------------------------------------------------------

unsigned int	Enemy::get_gold(void) const noexcept
{
	return (gold);
}

// Utils ----------------------------------------------------------------------

void	Enemy::on_interact(Player& player)
{
	// TODO: Logic ...
}

FighterType	Enemy::get_type(void) const noexcept
{
	return (FighterType::Enemy);
}

#include "characters/Enemy.hpp"

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

void	Enemy::on_interact(Player& player)
{
	// TODO: Logic ...
}

FighterType	Enemy::get_type(void) const noexcept
{
	return (FighterType::Enemy);
}

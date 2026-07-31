#include "characters/enemies/Enemy.hpp"

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
	// TODO: Logic ...
	return ("");
}

FighterType	Enemy::get_type(void) const noexcept
{
	return (FighterType::Enemy);
}

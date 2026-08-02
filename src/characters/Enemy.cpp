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

// MODIFIED: era "void", no coincidía con la virtual pura de Character
// (const std::string) -- no compilaba.
const std::string	Enemy::on_talk(Player& player) noexcept
{
	(void)player;
	// TODO: Logic ...
	return ("");
}

FighterType	Enemy::get_type(void) const noexcept
{
	return (FighterType::Enemy);
}

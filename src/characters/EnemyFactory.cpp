#include "characters/EnemyFactory.hpp"

#include <stdexcept>

#include "utils/utils.hpp"
#include "characters/Goblin.hpp"

Enemy	*EnemyFactory::create_from_name(const std::string& enemy_name)
{
	// ? REVIEW: Should trim_str be used?
	std::string	temp = enemy_name;

	trim_str(temp);
	if (temp == Goblin::NAME)
		return (create_goblin());
	// TODO: Add more enemies.
	throw std::invalid_argument("No enemy matches the name '" + enemy_name + "'.");
}

// Specific enemies -----------------------------------------------------------

Goblin	*EnemyFactory::create_goblin(void) noexcept
{
	return (new Goblin());
}

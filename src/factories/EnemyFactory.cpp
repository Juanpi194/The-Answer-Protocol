#include "factories/EnemyFactory.hpp"

#include <stdexcept>

#include "utils/utils.hpp"
#include "characters/enemies/Goblin.hpp"

Enemy	*EnemyFactory::create_from_name(const std::string& enemy_name)
{
	std::string	temp = enemy_name;

	trim_str(temp);
	if (temp == Goblin::NAME)
		return (create_goblin());
	throw std::invalid_argument("No enemy matches the name '" + enemy_name + "'.");
}

// Specific enemies -----------------------------------------------------------

Goblin	*EnemyFactory::create_goblin(void) noexcept
{
	return (new Goblin());
}

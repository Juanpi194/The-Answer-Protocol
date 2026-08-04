#include "factories/EnemyFactory.hpp"

#include <stdexcept>

#include "utils/utils.hpp"
#include "characters/enemies/Goblin.hpp"
#include "characters/enemies/Ogre.hpp"
#include "characters/enemies/Shade.hpp"
#include "characters/enemies/Wizard.hpp"
#include "characters/enemies/Shadow.hpp"
#include "characters/enemies/Dragon.hpp"

Enemy	*EnemyFactory::create_from_name(const std::string& enemy_name)
{
	std::string	temp = enemy_name;

	trim_str(temp);
	temp = to_upper(temp);
	if (temp == to_upper(Goblin::NAME))
		return (create_goblin());
	if (temp == to_upper(Ogre::NAME))
		return (create_ogre());
	if (temp == to_upper(Shade::NAME))
		return (create_shade());
	if (temp == to_upper(Wizard::NAME))
		return (create_wizard());
	if (temp == to_upper(Shadow::NAME))
		return (create_shadow());
	if (temp == to_upper(Dragon::NAME))
		return (create_dragon());
	throw std::invalid_argument("No enemy matches the name '" + enemy_name + "'.");
}

// Specific enemies -----------------------------------------------------------

Goblin	*EnemyFactory::create_goblin(void) noexcept
{
	return (new Goblin());
}

Ogre	*EnemyFactory::create_ogre(void) noexcept
{
	return (new Ogre());
}

Shade	*EnemyFactory::create_shade(void) noexcept
{
	return (new Shade());
}

Wizard	*EnemyFactory::create_wizard(void) noexcept
{
	return (new Wizard());
}

Shadow	*EnemyFactory::create_shadow(void) noexcept
{
	return (new Shadow());
}

Dragon	*EnemyFactory::create_dragon(void) noexcept
{
	return (new Dragon());
}

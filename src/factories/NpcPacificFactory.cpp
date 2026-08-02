#include "factories/NpcPacificFactory.hpp"

#include <stdexcept>

#include "utils/utils.hpp"
#include "characters/enemies/Goblin.hpp"

NPC	*NpcPacificFactory::create_from_name(const std::string& npc_name)
{
	// ? REVIEW: Should trim_str be used?
	std::string	temp = npc_name;

	trim_str(temp);
	if (temp == Enchanter::NAME)
		return (create_enchanter());
	// TODO: Add more enemies.
	throw std::invalid_argument("No enemy matches the name '" + npc_name + "'.");
}

// Specific enemies -----------------------------------------------------------

Goblin	*NpcFactory::create_enchanter(void) noexcept
{
	return (new Enchanter());
}

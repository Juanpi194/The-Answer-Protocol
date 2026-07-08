#include "characters/Enchanter.hpp"

#include <stdexcept>

#include "enchantments/Enchantment.hpp"

// Constructors ---------------------------------------------------------------

Enchanter::Enchanter(const std::string& id, const std::string& name, const std::string& description, const std::map<Enchantment*, unsigned int>& enchantments):
	Character(name),
	NPC(id, name, description),
	enchantments(enchantments)
{
	// TODO: Validate enchantments map
}

Enchanter::~Enchanter(void)
{
	// TODO: Free all enchantments from the map
}

// Getters and setters --------------------------------------------------------

void	Enchanter::on_interact(Player& player)
{
	// TODO: Logic...
}

void	Enchanter::enchant(Gear& gear, Enchantment *enchantment)
{
	if (!enchantment)
		throw std::invalid_argument("Enchantment cannot be nullptr.");
	// TODO: Logic...
}

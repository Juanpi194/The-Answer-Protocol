#include "characters/Enchanter.hpp"

#include <stdexcept>

#include "enchantments/Enchantment.hpp"
#include "items/Gear.hpp"

// Constructors ---------------------------------------------------------------

Enchanter::Enchanter(const std::string& name, const std::string& description, const std::map<Enchantment*, unsigned int>& enchantments):
	Character(name),
	NPC(name, description),
	enchantments(enchantments)
{
	for (std::pair<Enchantment*, unsigned int> enchantment_and_price: enchantments)
	{
		if (!enchantment_and_price.first)
			throw std::invalid_argument("Enchanter's enchantments list to sell cannot have any nullptr in it.");
	}
}

Enchanter::~Enchanter(void)
{
	for (std::pair<Enchantment*, unsigned int> enchantment_and_price: enchantments)
		delete (enchantment_and_price.first);
}

// Getters and setters --------------------------------------------------------

const std::map<Enchantment*, unsigned int>&	Enchanter::get_enchantments(void) const noexcept
{
	return (enchantments);
}

// Utils ----------------------------------------------------------------------

void	Enchanter::on_interact(Player& player)
{
	// TODO: Logic...
}

void	Enchanter::enchant(Gear& gear, Enchantment *enchantment)
{
	// ? REVIEW: Check this method logic (probably redo it).
	bool	allowed;

	allowed = false;
	if (!enchantment)
		throw std::invalid_argument("Enchantment cannot be nullptr.");
	for (GearType gear_type: enchantment->get_allowed_gear_types())
	{
		if (gear_type == gear.get_gear_type())
			allowed = true;
	}
	if (!allowed)
		return ;	// ! FIXME: Logic if the gear type is not allowed in this enchantment.
	gear.set_enchantment(enchantment);
}

#include "characters/Enchanter.hpp"

#include <stdexcept>

#include "enchantments/Enchantment.hpp"
#include "items/Gear.hpp"
#include "characters/Player.hpp"

unsigned int		Enchanter::available_id = 0;
const std::string	Enchanter::PREFIX = "enchanter.";

// Constructors ---------------------------------------------------------------

Enchanter::Enchanter(const std::string& name, const std::string& description, const std::map<Enchantment*, unsigned int>& enchantments_to_sell):
	Character(name),
	NPC(NPC::PREFIX + PREFIX + std::to_string(available_id++), name, description),
	enchantments_to_sell(enchantments_to_sell)
{
	if (enchantments_to_sell.size() == 0)
		throw std::invalid_argument("Enchanter's enchantments list to sell cannot be empty.");
	for (std::pair<Enchantment*, unsigned int> enchantment_and_price: enchantments_to_sell)
	{
		if (!enchantment_and_price.first)
			throw std::invalid_argument("Enchanter's enchantments list to sell cannot have any nullptr in it.");
	}
}

Enchanter::~Enchanter(void)
{
	for (std::pair<Enchantment*, unsigned int> enchantment_and_price: enchantments_to_sell)
		delete (enchantment_and_price.first);
}

// Getters and setters --------------------------------------------------------

const std::map<Enchantment*, unsigned int>&	Enchanter::get_enchantments_to_sell(void) const noexcept
{
	return (enchantments_to_sell);
}

// Utils ----------------------------------------------------------------------

void	Enchanter::on_interact(Player& player)
{
	std::string	products;

	// ? REVIEW: Logic and format ...
	products = "===";
	player.send_to_client("Welcome!");
	for (std::pair<Enchantment*, unsigned int> enchantment_and_price: enchantments_to_sell)
	{
		products += "\n";
		products += (enchantment_and_price.first->get_name() + " - " + std::to_string(enchantment_and_price.second));
	}
	products += "\n===";
	player.send_to_client(products);
	player.receive_command();
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

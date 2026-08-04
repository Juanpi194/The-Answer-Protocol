#include "characters/Enchanter.hpp"

#include <stdexcept>

#include "utils/utils.hpp"
#include "enchantments/Enchantment.hpp"
#include "items/Gear.hpp"
#include "characters/Player.hpp"

unsigned int		Enchanter::available_id = 0;
const std::string	Enchanter::PREFIX = "enchanter.";

bool	Enchanter::enchant(Gear& gear, Enchantment& enchantment)
{
	bool	allowed;

	allowed = false;
	for (GearType gear_type: enchantment.get_allowed_gear_types())
		if (gear_type == gear.get_gear_type())
			allowed = true;
	if (!allowed)
		return (log("Cannot apply '" + enchantment.get_name() + "' to '" + gear.get_name() + "'.", LogLevel::INFO), false);
	gear.set_enchantment(enchantment.clone());
	return (true);
}

// Constructors ---------------------------------------------------------------

Enchanter::Enchanter(const std::string& name, const std::string& description, const std::map<Enchantment*, unsigned int>& enchantments_to_sell):
	Character(name),
	NPC(NPC::PREFIX + PREFIX + std::to_string(available_id++), name, description),
	enchantments_to_sell(enchantments_to_sell)
{
	if (enchantments_to_sell.size() == 0)
		throw std::invalid_argument("Enchanter's enchantments list to sell cannot be empty.");
	for (std::pair<Enchantment*, unsigned int> enchantment_and_price: enchantments_to_sell)
		if (!enchantment_and_price.first)
			throw std::invalid_argument("Enchanter's enchantments list to sell cannot have any nullptr in it.");
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

const std::string	Enchanter::on_talk(Player& player) noexcept
{
	std::string	products;

	// ? REVIEW: Logic and format ...
	products = "===";
	products += "Welcome!";
	for (std::pair<Enchantment*, unsigned int> enchantment_and_price: enchantments_to_sell)
	{
		products += "\n";
		products += (enchantment_and_price.first->get_name() + " - " + std::to_string(enchantment_and_price.second));
	}
	products += "\n===";
	return (products);
	// TODO: Logic...
}

bool	Enchanter::on_enchant(Player &player, const std::string& gear, const std::string& enchantment)
{
	Item		*found_item;
	Gear		*found_gear;
	Enchantment	*found_enchantment;

	found_item = nullptr;
	found_gear = nullptr;
	for (Item *item: player.get_inventory().get_items())
	{
		if (item->get_name() == gear)
		{
			found_item = item;
			found_gear = dynamic_cast<Gear*>(item);
			if (found_gear && found_gear->get_enchantment())
				found_gear = nullptr;
			else if (found_gear)
				break;
		}
	}
	if (!found_item)
		return (false);
	if (!found_gear)
		return (false);
	found_enchantment = player.find_enchantment_by_name(enchantment);
	if (!found_enchantment)
		return (false);
	if (!enchant(*found_gear, *found_enchantment))
		return (false);
	player.consume_enchantment(*found_enchantment);
	return (true);
}

bool	Enchanter::on_buy(Player& player, const std::string& product) noexcept
{
	Enchantment		*enchantment_found;
	unsigned int	price;

	enchantment_found = nullptr;
	for (std::pair<Enchantment*, unsigned int> enchantment_and_price: enchantments_to_sell)
	{
		if (enchantment_and_price.first->get_name() == product)
		{
			enchantment_found = enchantment_and_price.first;
			price = enchantment_and_price.second;
		}
	}
	if (!enchantment_found)
		return (false);
	if (!player.spend_gold(price))
		return (false);
	player.add_enchantment(enchantment_found->clone());
	return (true);
}

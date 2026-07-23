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
	{
		if (gear_type == gear.get_gear_type())
			allowed = true;
	}
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

void	Enchanter::on_talk(Player& player) noexcept
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
	// TODO: Logic...
}

void	Enchanter::on_enchant(Player &player, const std::string& gear, const std::string& enchantment)
{
	Item		*found_item;
	Gear		*found_gear;
	Enchantment	*found_enchantment;

	found_item = player.get_inventory().find_item_by_name(gear);
	if (!found_item)
	{
		log("Couldn't find the item '" + gear + "' in '" + player.get_name() + "' item list.", LogLevel::WARNING);
		player.send_to_client("You do not have that item in your bag.");
		return ;
	}
	found_gear = dynamic_cast<Gear*>(found_item);
	if (!found_gear)
	{
		log("Item '" + gear + "' from '" + player.get_name() + "' is not a gear, couldn't be enchanted.", LogLevel::WARNING);
		player.send_to_client("The item you specified is not gear.");
		return ;
	}
	if (found_gear->get_enchantment())
	{
		log("Gear '" + gear + "' already has an enchantment. " + enchantment + " was not applied.", LogLevel::INFO);
		player.send_to_client("Your " + gear + " already has an enchantment.");
		return ;
	}
	found_enchantment = player.find_enchantment_by_name(enchantment);
	if (!found_enchantment)
	{
		log("Couldn't find the enchantment '" + enchantment + "' in '" + player.get_name() + "' enchantment list.", LogLevel::WARNING);
		player.send_to_client("You do not have that enchantment in your bag.");
		return ;
	}
	player.send_to_client("Let's try to apply '" + found_enchantment->get_name() + "' to your '" + found_gear->get_name() + "'.");
	if (!enchant(*found_gear, *found_enchantment))
		player.send_to_client("I cannot apply that enchantment to your '" + found_gear->get_name() + "'.");
	else
	{
		player.send_to_client("I applied the enchantment to your '" + found_gear->get_name() + "'.");
		player.consume_enchantment(*found_enchantment);
	}
}

void	Enchanter::on_buy(Player& player, const std::string& product) noexcept
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
	{
		log("Enchantment '" + product + "' is not sold at '" + get_name() + "'s shop.", LogLevel::INFO);
		player.send_to_client("We don't sell '" + product + "' here.");
		return ;
	}
	if (!player.spend_gold(price))
		player.send_to_client("You don't have enough money for that.");
	else
	{
		player.send_to_client("Here you go.");
		player.add_enchantment(enchantment_found->clone());
	}
}

#include "characters/Merchant.hpp"

#include <stdexcept>

#include "utils/utils.hpp"
#include "items/Item.hpp"
#include "characters/Player.hpp"

unsigned int		Merchant::available_id = 0;
const std::string	Merchant::PREFIX = "merchant.";

// Constructors ---------------------------------------------------------------

Merchant::Merchant(const std::string& name, const std::string& description, const std::map<Item*, unsigned int>& items_to_sell):
	Character(name),
	NPC(NPC::PREFIX + PREFIX + std::to_string(available_id++), name, description),
	items_to_sell(items_to_sell)
{
	if (items_to_sell.empty())
		throw std::invalid_argument("Merchant's item list to sell cannot be empty.");
	for (const std::pair<Item*, unsigned int>& item_and_price: items_to_sell)
	{
		if (!item_and_price.first)
			throw std::invalid_argument("Merchant's item list to sell cannot have any nullptr in it.");
	}
}

Merchant::~Merchant(void)
{
	for (const std::pair<Item*, unsigned int>& item_and_price: items_to_sell)
		delete (item_and_price.first);
}

// Getters and setters --------------------------------------------------------

const std::map<Item*, unsigned int>&	Merchant::get_items_to_sell(void) const noexcept
{
	return (items_to_sell);
}

// Utils ----------------------------------------------------------------------

void	Merchant::on_talk(Player& player) noexcept
{
	std::string	products;

	// ? REVIEW: Logic and format ...
	products = "===";
	player.send_to_outbox("Welcome!");
	for (std::pair<Item*, unsigned int> item_and_price: items_to_sell)
	{
		products += "\n";
		products += (item_and_price.first->get_name() + " - " + std::to_string(item_and_price.second));
	}
	products += "\n===";
	player.send_to_outbox(products);
	// TODO: Logic...
}

void	Merchant::on_buy(Player& player, const std::string& product) noexcept
{
	Item			*item_found;
	unsigned int	price;

	item_found = nullptr;
	for (std::pair<Item*, unsigned int> item_and_price: items_to_sell)
	{
		if (item_and_price.first->get_name() == product)
		{
			item_found = item_and_price.first;
			price = item_and_price.second;
		}
	}
	if (!item_found)
	{
		log("Item '" + product + "' is not sold at '" + get_name() + "'s shop.", LogLevel::INFO);
		player.send_to_outbox("We don't sell '" + product + "' here.");
		return ;
	}
	if (!player.spend_gold(price))
		player.send_to_outbox("You don't have enough money for that.");
	else
	{
		player.send_to_outbox("Here you go.");
		player.get_inventory().add_item(item_found->clone());
	}
}

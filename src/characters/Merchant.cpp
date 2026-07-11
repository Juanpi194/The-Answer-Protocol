#include "characters/Merchant.hpp"

#include <stdexcept>

#include "items/Item.hpp"
#include "characters/Player.hpp"

// Constructors ---------------------------------------------------------------

Merchant::Merchant(const std::string& name, const std::string& description, const std::map<Item*, unsigned int>& items_to_sell):
	Character(name),
	NPC(name, description),
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

void	Merchant::on_interact(Player& player)
{
	std::string	products;

	// ? REVIEW: Logic and format ...
	products = "===";
	player.send_to_client("Welcome!");
	for (std::pair<Item*, unsigned int> item_and_price: items_to_sell)
	{
		products += "\n";
		products += (item_and_price.first->get_name() + " - " + std::to_string(item_and_price.second));
	}
	products += "\n===";
	player.send_to_client(products);
	player.receive_command();
	// TODO: Logic...
}

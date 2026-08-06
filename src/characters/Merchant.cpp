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
	for (const std::pair<Item* const, unsigned int>& item_and_price: items_to_sell)
		if (!item_and_price.first)
			throw std::invalid_argument("Merchant's item list to sell cannot have any nullptr in it.");
}

Merchant::~Merchant(void)
{
	for (const std::pair<Item* const, unsigned int>& item_and_price: items_to_sell)
		delete (item_and_price.first);
}

// Getters and setters --------------------------------------------------------

const std::map<Item*, unsigned int>&	Merchant::get_items_to_sell(void) const noexcept
{
	return (items_to_sell);
}

// Utils ----------------------------------------------------------------------

const std::string	Merchant::on_talk(Player& player) noexcept
{
	std::string	result;
	bool		first;

	(void)player;
	result  = "{";
	result += "\"npc\": \"" + get_name() + "\", ";
	result += "\"role\": \"merchant\", ";
	result += "\"items\": [";
	first = true;
	for (std::pair<Item* const, unsigned int> entry : items_to_sell)
	{
		if (!first)
			result += ", ";
		result += "{";
		result += "\"name\": \"" + entry.first->get_name() + "\", ";
		result += "\"price\": " + std::to_string(entry.second) + ", ";
		result += "\"description\": \"" + entry.first->get_description() + "\"";
		result += "}";
		first = false;
	}
	result += "]}";
	return (result);
}

bool	Merchant::on_buy(Player& player, const std::string& product) noexcept
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
		return (false);
	if (!player.spend_gold(price))
		return (false);
	player.get_inventory().add_item(item_found->clone());
	return (true);
}

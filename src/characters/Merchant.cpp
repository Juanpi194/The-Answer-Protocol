#include "characters/Merchant.hpp"

#include <iostream>
#include <cctype>

#include "items/Item.hpp"

// Constructors ---------------------------------------------------------------

Merchant::Merchant(const std::string& id, const std::string& name, const std::string& description, const std::map<Item*, unsigned int>& items_to_sell):
	Character(name),
	NPC(id, name, description),
	items_to_sell(items_to_sell)
{
	// ? REVIEW: Is the validation correct?
	for (const std::pair<Item*, unsigned int>& item_and_price: items_to_sell)
	{
		if (!item_and_price.first)
			throw std::invalid_argument("Merchant's item list to sell cannot have any nullptr in it.");
	}
}

Merchant::~Merchant(void)
{
	// ? REVIEW: Is it the delete logic correct?
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
	// TODO: Logic ...
}

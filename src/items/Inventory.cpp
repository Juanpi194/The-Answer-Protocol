#include "items/Inventory.hpp"

#include <stdexcept>

#include "utils/utils.hpp"
#include "items/Item.hpp"

// Constructors ---------------------------------------------------------------

Inventory::~Inventory(void)
{
	for (Item *item: items)
		delete(item);
}

// Getters and setters --------------------------------------------------------

const std::list<Item*>&	Inventory::get_items(void) const noexcept
{
	return (items);
}

// Utils ----------------------------------------------------------------------

void	Inventory::add_item(Item *item) noexcept
{
	assert(item != nullptr && "Cannot add a nullptr to the item list.");
	items.push_back(item);
}

void	Inventory::remove_item(Item *item) noexcept
{
	assert(item != nullptr && "Cannot remove a nullptr from the item list.");
	items.remove(item);
}

void	Inventory::consume_item(Item& item)
{
	Item	*item_found;

	item_found = nullptr;
	for (Item *item_in_list: items)
	{
		if (item_in_list == &item)
			item_found = item_in_list;
	}
	if (!item_found)
		throw std::invalid_argument("Item to consume is not in the player's item list.");

	// Removing the item
	items.remove(item_found);
	delete (item_found);
}

Item	*Inventory::find_item_by_name(const std::string& item_name) const
{
	for (Item *item_in_list: items)
	{
		if (item_in_list->get_name() == item_name)
			return (item_in_list);
	}
	return (nullptr);
}

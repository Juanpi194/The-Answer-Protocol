#include "items/Chest.hpp"

#include <cstdlib>
#include <stdexcept>

#include "items/ItemFactory.hpp"
#include "utils/utils.hpp"
#include "characters/Player.hpp"

std::list<Item*>	Chest::open(void) noexcept
{
	std::list<Item*>	generated_items;
	int					random;

	if (opened)
		return (generated_items);
	opened = true;
	// TODO: Opening chest logic, this leaves leaks.
	// while (generated_items.size() == 0)
	// {
	// 	for (std::pair<Item*, unsigned int> item_and_chance: pool)
	// 	{
	// 		random = rand() % 100;
	// 		if (random < item_and_chance.second)
	// 		{
	// 			generated_items.push_back(item_and_chance.first);
	// 			item_and_chance.first = nullptr;
	// 		}
	// 	}
	// }
	log("Chest opened.", LogLevel::DEBUG);
	return (generated_items);
}

// Constructors ---------------------------------------------------------------

Chest::Chest(void):
	opened(false),
	pool(ItemFactory::create_default_pool())
{
	for (std::pair<Item*, unsigned int> item_and_chance: pool)
	{
		if (!item_and_chance.first)
			throw std::invalid_argument("The pool cannot have nullptr items.");
		if (item_and_chance.second == 0)
			throw std::invalid_argument("The pool cannot have 0 as the chance of any item.");
	}
}

Chest::~Chest(void)
{
	for (std::pair<Item*, unsigned int> item_and_chance: pool)
		delete (item_and_chance.first);
}

// Getters and setters --------------------------------------------------------

bool									Chest::is_opened(void) const noexcept
{
	return (opened);
}

const std::map<Item*, unsigned int>&	Chest::get_pool(void) const noexcept
{
	return (pool);
}

// Utils ----------------------------------------------------------------------

std::list<Item*>	Chest::interact(Player& player) noexcept
{
	std::list<Item*>	result;

	if (opened)
		return (result);
	// TODO: Check if player has key, open if so. Return empty list otherwise.
	// if (no key)
	// 	return (log("Player '" + player.get_name() + "' has no key to open the chest.", LogLevel::INFO), result);
	result = open();
	return (result);
}

#include "items/Chest.hpp"

#include <cstdlib>
#include <stdexcept>

#include "items/ChestKey.hpp"
#include "items/ItemFactory.hpp"
#include "utils/utils.hpp"
#include "characters/Enemy.hpp"
#include "characters/Player.hpp"

std::list<Item*>	Chest::open(void)
{
	std::list<Item*>	generated_items;
	int					random;

	if (opened)
		return (generated_items);
	opened = true;

	// ? REVIEW: Instead of making it a loop until one item is generated,
	// ?		 we could give a default item (maybe a potion) if no item is
	// ?		 is generated.
	// Generating items
    while (generated_items.size() == 0)
	{
		for (std::pair<Item* const, unsigned int>& entry : pool)
		{
			random = rand() % 100;
			if (random < entry.second)
				generated_items.push_back(entry.first);
		}
	}

	// Cleaning the chest to avoid double delete
	for (Item *item: generated_items)
		pool.erase(item);
	log("Chest opened.", LogLevel::DEBUG);
	return (generated_items);
}

// Constructors ---------------------------------------------------------------

Chest::Chest(Enemy *guardian):
	opened(false),
	pool(ItemFactory::create_default_pool()),
	guardian(guardian)
{
	if (pool.size() == 0)
		throw std::runtime_error("The pool cannot be empty. Check ItemFactory.cpp method.");
	for (const std::pair<Item* const, unsigned int>& item_and_chance: pool)
	{
		if (!item_and_chance.first)
			throw std::invalid_argument("The pool cannot have nullptr items.");
		if (item_and_chance.second == 0)
			throw std::invalid_argument("The pool cannot have 0 as the chance of any item.");
		if (item_and_chance.second > 100)
			throw std::invalid_argument("The pool cannot have more than 100 as the chance of an item.");
	}
}

Chest::~Chest(void)
{
	for (std::pair<Item* const, unsigned int> item_and_chance: pool)
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

Enemy									*Chest::get_guardian(void) const noexcept
{
	return (guardian);
}

// Utils ----------------------------------------------------------------------

std::list<Item*>	Chest::interact(Player& player)
{
	std::list<Item*>	result;
	ChestKey			*chest_key;

	if (opened)
		return (result);
	if (guardian && !player.is_enemy_beaten(guardian))
		return (log("Player '" + player.get_name() + "' has to beat '" + guardian->get_name() + "' before opening the chest.", LogLevel::WARNING), result);
	chest_key = player.get_inventory().find_item<ChestKey>();
	if (!chest_key)
		return (log("Player '" + player.get_name() + "' has no key to open the chest.", LogLevel::INFO), result);

	// Removing the key
	player.get_inventory().consume_item(*chest_key);

	// ? REVIEW: Maybe instead of adding the items to the room,
	// ?		 we should add them to the player.
	result = open();
	for (Item *item: result)
		player.get_inventory().add_item(item);
	return (result);
}

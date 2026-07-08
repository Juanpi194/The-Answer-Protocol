#include "items/Chest.hpp"

std::list<Item*>	Chest::open(void) noexcept
{
	std::list<Item*>	generated_items;

	opened = true;
	// TODO: Randomly, generate items from the pool.
	return (generated_items);
}

// Constructors ---------------------------------------------------------------

Chest::Chest(void):
	opened(false)
{
}

// Getters and setters --------------------------------------------------------

// TODO: Add item pool getter

// Utils ----------------------------------------------------------------------

std::list<Item*>	*Chest::interact(Player& player) noexcept
{
	if (opened)
		return (nullptr);
	// TODO: Check if player has key, open if so. Return nullptr otherwise.
	return (nullptr);
}

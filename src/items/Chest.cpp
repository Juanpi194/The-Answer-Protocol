#include "items/Chest.hpp"

#include "items/ItemFactory.hpp"

std::list<Item*>	Chest::open(void) noexcept
{
	std::list<Item*>	generated_items;

	opened = true;
	// TODO: Randomly, generate items from the pool.
	return (generated_items);
}

// Constructors ---------------------------------------------------------------

Chest::Chest(void):
	opened(false),
	pool(ItemFactory::create_default_pool())
{
	// TODO: Check pool sum (It should get to 100).
}

// Getters and setters --------------------------------------------------------

bool								Chest::is_opened(void) const noexcept
{
	return (opened);
}

const std::map<Item*, unsigned int>	Chest::get_pool(void) const noexcept
{
	return (pool);
}

// Utils ----------------------------------------------------------------------

std::list<Item*>	*Chest::interact(Player& player) noexcept
{
	if (opened)
		return (nullptr);
	// TODO: Check if player has key, open if so. Return nullptr otherwise.
	return (nullptr);
}

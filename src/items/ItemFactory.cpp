#include "items/ItemFactory.hpp"

#include "items/IronSword.hpp"

std::map<Item*, unsigned int>	ItemFactory::create_default_pool(void)
{
	std::map<Item*, unsigned int>	pool;

	// TODO: Add more items to the pool
    pool[create_iron_sword()] = 100;
	return (pool);
}

IronSword						*ItemFactory::create_iron_sword(void)
{
	return (new IronSword());
}

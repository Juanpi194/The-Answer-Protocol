#include "items/ItemFactory.hpp"

#include "items/IronArmor.hpp"
#include "items/IronShield.hpp"
#include "items/IronSword.hpp"

std::map<Item*, unsigned int>	ItemFactory::create_default_pool(void) noexcept
{
	std::map<Item*, unsigned int>	pool;

	// TODO: Add more items to the pool
	pool[create_iron_armor()] = 100;
	pool[create_iron_shield()] = 100;
    pool[create_iron_sword()] = 100;
	return (pool);
}

IronArmor						*ItemFactory::create_iron_armor(void) noexcept
{
	return (new IronArmor());
}

IronShield						*ItemFactory::create_iron_shield(void) noexcept
{
	return (new IronShield());
}

IronSword						*ItemFactory::create_iron_sword(void) noexcept
{
	return (new IronSword());
}

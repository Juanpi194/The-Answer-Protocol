#include "items/ItemFactory.hpp"

#include <stdexcept>

#include "utils/utils.hpp"
#include "items/ChestKey.hpp"
#include "items/IronArmor.hpp"
#include "items/IronShield.hpp"
#include "items/IronSword.hpp"

std::map<Item*, unsigned int>	ItemFactory::create_default_pool(void) noexcept
{
	std::map<Item*, unsigned int>	pool;

	// TODO: Add more items to the pool
	pool[create_iron_armor()] = 30;
	pool[create_iron_shield()] = 30;
    pool[create_iron_sword()] = 30;
	return (pool);
}

Item							*ItemFactory::create_from_name(const std::string& item_name)
{
	// ? REVIEW: Should trim_str be used?
	std::string	temp = item_name;

	trim_str(temp);
	if (temp == ChestKey::NAME)
		return (create_chest_key());
	if (temp == IronArmor::NAME)
		return (create_iron_armor());
	if (temp == IronShield::NAME)
		return (create_iron_shield());
	if (temp == IronSword::NAME)
		return (create_iron_sword());
	// TODO: Add more items.
	throw std::invalid_argument("No item matches the name '" + item_name + "'.");
}

// Specific items -------------------------------------------------------------

ChestKey	*ItemFactory::create_chest_key(void) noexcept
{
	return (new ChestKey());
}

IronArmor	*ItemFactory::create_iron_armor(void) noexcept
{
	return (new IronArmor());
}

IronShield	*ItemFactory::create_iron_shield(void) noexcept
{
	return (new IronShield());
}

IronSword	*ItemFactory::create_iron_sword(void) noexcept
{
	return (new IronSword());
}

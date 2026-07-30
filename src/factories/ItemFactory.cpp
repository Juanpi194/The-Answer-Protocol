#include "factories/ItemFactory.hpp"

#include <stdexcept>

#include "utils/utils.hpp"
#include "items/Apple.hpp"
#include "items/ChestKey.hpp"
#include "items/FlameSword.hpp"
#include "items/IronArmor.hpp"
#include "items/IronShield.hpp"
#include "items/IronSword.hpp"
#include "items/SpikeArmor.hpp"

std::map<Item*, unsigned int>	ItemFactory::create_default_pool(void) noexcept
{
	std::map<Item*, unsigned int>	pool;

	// TODO: Add more items to the pool
	pool[create_apple()] = 50;
	pool[create_flame_sword()] = 10;
	pool[create_iron_armor()] = 30;
	pool[create_iron_shield()] = 30;
	pool[create_iron_sword()] = 30;
	pool[create_spike_armor()] = 10;
	return (pool);
}

Item							*ItemFactory::create_from_name(const std::string& item_name)
{
	// ? REVIEW: Should trim_str be used?
	std::string	temp = item_name;

	trim_str(temp);
	if (temp == Apple::NAME)
		return (create_apple());
	if (temp == ChestKey::NAME)
		return (create_chest_key());
	if (temp == FlameSword::NAME)
		return (create_flame_sword());
	if (temp == IronArmor::NAME)
		return (create_iron_armor());
	if (temp == IronShield::NAME)
		return (create_iron_shield());
	if (temp == IronSword::NAME)
		return (create_iron_sword());
	if (temp == SpikeArmor::NAME)
		return (create_spike_armor());
	// TODO: Add more items.
	throw std::invalid_argument("No item matches the name '" + item_name + "'.");
}

// Specific items -------------------------------------------------------------

Apple		*ItemFactory::create_apple(void) noexcept
{
	return (new Apple());
}

ChestKey	*ItemFactory::create_chest_key(void) noexcept
{
	return (new ChestKey());
}

FlameSword	*ItemFactory::create_flame_sword(void) noexcept
{
	return (new FlameSword());
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

SpikeArmor	*ItemFactory::create_spike_armor(void) noexcept
{
	return (new SpikeArmor());
}

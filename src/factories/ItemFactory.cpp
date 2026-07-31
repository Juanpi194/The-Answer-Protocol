#include "factories/ItemFactory.hpp"

#include <stdexcept>

#include "utils/utils.hpp"
#include "items/consumables/Apple.hpp"
#include "items/armor/BronzeArmor.hpp"
#include "items/weapons/BronzeShield.hpp"
#include "items/weapons/BronzeSword.hpp"
#include "items/chest/ChestKey.hpp"
#include "items/armor/EmberArmor.hpp"
#include "items/consumables/FirePotion.hpp"
#include "items/weapons/FlameSword.hpp"
#include "items/armor/FrostArmor.hpp"
#include "items/weapons/FrostSword.hpp"
#include "items/consumables/HastePotion.hpp"
#include "items/consumables/IcePotion.hpp"
#include "items/armor/IronArmor.hpp"
#include "items/weapons/IronShield.hpp"
#include "items/weapons/IronSword.hpp"
#include "items/consumables/PoisonPotion.hpp"
#include "items/weapons/SerratedSword.hpp"
#include "items/armor/SpikeArmor.hpp"
#include "items/armor/SteelArmor.hpp"
#include "items/weapons/SteelShield.hpp"
#include "items/weapons/SteelSword.hpp"

std::map<Item*, unsigned int>	ItemFactory::create_default_pool(void) noexcept
{
	std::map<Item*, unsigned int>	pool;

	// TODO: Add more items to the pool
	pool[create_apple()] = 50;
	pool[create_bronze_armor()] = 30;
	pool[create_bronze_shield()] = 30;
	pool[create_bronze_sword()] = 30;
	pool[create_ember_armor()] = 10;
	pool[create_fire_potion()] = 30;
	pool[create_flame_sword()] = 10;
	pool[create_frost_armor()] = 10;
	pool[create_frost_sword()] = 10;
	pool[create_haste_potion()] = 30;
	pool[create_ice_potion()] = 30;
	pool[create_iron_armor()] = 30;
	pool[create_iron_shield()] = 30;
	pool[create_iron_sword()] = 30;
	pool[create_poison_potion()] = 30;
	pool[create_serrated_sword()] = 10;
	pool[create_spike_armor()] = 10;
	pool[create_steel_armor()] = 15;
	pool[create_steel_shield()] = 30;
	pool[create_steel_sword()] = 15;
	return (pool);
}

Item							*ItemFactory::create_from_name(const std::string& item_name)
{
	// ? REVIEW: Should trim_str be used?
	std::string	temp = item_name;

	trim_str(temp);
	if (temp == Apple::NAME)
		return (create_apple());
	if (temp == BronzeArmor::NAME)
		return (create_bronze_armor());
	if (temp == BronzeShield::NAME)
		return (create_bronze_shield());
	if (temp == BronzeSword::NAME)
		return (create_bronze_sword());
	if (temp == ChestKey::NAME)
		return (create_chest_key());
	if (temp == EmberArmor::NAME)
		return (create_ember_armor());
	if (temp == FirePotion::NAME)
		return (create_fire_potion());
	if (temp == FlameSword::NAME)
		return (create_flame_sword());
	if (temp == FrostArmor::NAME)
		return (create_frost_armor());
	if (temp == FrostSword::NAME)
		return (create_frost_sword());
	if (temp == HastePotion::NAME)
		return (create_haste_potion());
	if (temp == IcePotion::NAME)
		return (create_ice_potion());
	if (temp == IronArmor::NAME)
		return (create_iron_armor());
	if (temp == IronShield::NAME)
		return (create_iron_shield());
	if (temp == IronSword::NAME)
		return (create_iron_sword());
	if (temp == PoisonPotion::NAME)
		return (create_poison_potion());
	if (temp == SerratedSword::NAME)
		return (create_serrated_sword());
	if (temp == SpikeArmor::NAME)
		return (create_spike_armor());
	if (temp == SteelArmor::NAME)
		return (create_steel_armor());
	if (temp == SteelShield::NAME)
		return (create_steel_shield());
	if (temp == SteelSword::NAME)
		return (create_steel_sword());
	// TODO: Add more items.
	throw std::invalid_argument("No item matches the name '" + item_name + "'.");
}

// Specific items -------------------------------------------------------------

Apple		*ItemFactory::create_apple(void) noexcept
{
	return (new Apple());
}

BronzeArmor	*ItemFactory::create_bronze_armor(void) noexcept
{
	return (new BronzeArmor());
}

BronzeShield	*ItemFactory::create_bronze_shield(void) noexcept
{
	return (new BronzeShield());
}

BronzeSword	*ItemFactory::create_bronze_sword(void) noexcept
{
	return (new BronzeSword());
}

ChestKey	*ItemFactory::create_chest_key(void) noexcept
{
	return (new ChestKey());
}

EmberArmor	*ItemFactory::create_ember_armor(void) noexcept
{
	return (new EmberArmor());
}

FirePotion	*ItemFactory::create_fire_potion(void) noexcept
{
	return (new FirePotion());
}

FlameSword	*ItemFactory::create_flame_sword(void) noexcept
{
	return (new FlameSword());
}

FrostArmor	*ItemFactory::create_frost_armor(void) noexcept
{
	return (new FrostArmor());
}

FrostSword	*ItemFactory::create_frost_sword(void) noexcept
{
	return (new FrostSword());
}

HastePotion	*ItemFactory::create_haste_potion(void) noexcept
{
	return (new HastePotion());
}

IcePotion	*ItemFactory::create_ice_potion(void) noexcept
{
	return (new IcePotion());
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

PoisonPotion	*ItemFactory::create_poison_potion(void) noexcept
{
	return (new PoisonPotion());
}

SerratedSword	*ItemFactory::create_serrated_sword(void) noexcept
{
	return (new SerratedSword());
}

SpikeArmor	*ItemFactory::create_spike_armor(void) noexcept
{
	return (new SpikeArmor());
}

SteelArmor	*ItemFactory::create_steel_armor(void) noexcept
{
	return (new SteelArmor());
}

SteelShield	*ItemFactory::create_steel_shield(void) noexcept
{
	return (new SteelShield());
}

SteelSword	*ItemFactory::create_steel_sword(void) noexcept
{
	return (new SteelSword());
}

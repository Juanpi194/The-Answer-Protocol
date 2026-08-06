#include "factories/ItemFactory.hpp"

#include <stdexcept>

#include "utils/utils.hpp"
#include "items/armor/BronzeArmor.hpp"
#include "items/armor/EmberArmor.hpp"
#include "items/armor/FrostArmor.hpp"
#include "items/armor/IronArmor.hpp"
#include "items/armor/SpikeArmor.hpp"
#include "items/armor/SteelArmor.hpp"
#include "items/chest/ChestKey.hpp"
#include "items/consumables/Apple.hpp"
#include "items/consumables/FirePotion.hpp"
#include "items/consumables/HastePotion.hpp"
#include "items/consumables/IcePotion.hpp"
#include "items/consumables/PoisonPotion.hpp"
#include "items/shields/BronzeShield.hpp"
#include "items/shields/IronShield.hpp"
#include "items/shields/SpikeShield.hpp"
#include "items/shields/SteelShield.hpp"
#include "items/weapons/BronzeSword.hpp"
#include "items/weapons/FlameSword.hpp"
#include "items/weapons/FrostSword.hpp"
#include "items/weapons/IronSword.hpp"
#include "items/weapons/SerratedSword.hpp"
#include "items/weapons/SteelSword.hpp"
// MODIFIED: HealingPotion.hpp was not called
#include "items/consumables/HealingPotion.hpp"

std::map<Item*, unsigned int>	ItemFactory::create_default_pool(void)
{
	std::map<Item*, unsigned int>	pool;

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
	pool[create_spike_shield()] = 5;
	pool[create_steel_armor()] = 15;
	pool[create_steel_shield()] = 30;
	pool[create_steel_sword()] = 15;
	return (pool);
}

Item							*ItemFactory::create_from_name(const std::string& item_name)
{
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
	throw std::invalid_argument("No item matches the name '" + item_name + "'.");
}

// Specific items -------------------------------------------------------------

Apple			*ItemFactory::create_apple(void)
{
	return (new Apple());
}

BronzeArmor		*ItemFactory::create_bronze_armor(void)
{
	return (new BronzeArmor());
}

BronzeShield	*ItemFactory::create_bronze_shield(void)
{
	return (new BronzeShield());
}

BronzeSword		*ItemFactory::create_bronze_sword(void)
{
	return (new BronzeSword());
}

ChestKey		*ItemFactory::create_chest_key(void)
{
	return (new ChestKey());
}

EmberArmor		*ItemFactory::create_ember_armor(void)
{
	return (new EmberArmor());
}

FirePotion		*ItemFactory::create_fire_potion(void)
{
	return (new FirePotion());
}

FlameSword		*ItemFactory::create_flame_sword(void)
{
	return (new FlameSword());
}

FrostArmor		*ItemFactory::create_frost_armor(void)
{
	return (new FrostArmor());
}

FrostSword		*ItemFactory::create_frost_sword(void)
{
	return (new FrostSword());
}

HastePotion		*ItemFactory::create_haste_potion(void)
{
	return (new HastePotion());
}

IcePotion		*ItemFactory::create_ice_potion(void)
{
	return (new IcePotion());
}

IronArmor		*ItemFactory::create_iron_armor(void)
{
	return (new IronArmor());
}

IronShield		*ItemFactory::create_iron_shield(void)
{
	return (new IronShield());
}

IronSword		*ItemFactory::create_iron_sword(void)
{
	return (new IronSword());
}

PoisonPotion	*ItemFactory::create_poison_potion(void)
{
	return (new PoisonPotion());
}

SerratedSword	*ItemFactory::create_serrated_sword(void)
{
	return (new SerratedSword());
}

SpikeArmor		*ItemFactory::create_spike_armor(void)
{
	return (new SpikeArmor());
}

SpikeShield		*ItemFactory::create_spike_shield(void)
{
	return (new SpikeShield());
}

SteelArmor		*ItemFactory::create_steel_armor(void)
{
	return (new SteelArmor());
}

SteelShield		*ItemFactory::create_steel_shield(void)
{
	return (new SteelShield());
}

SteelSword		*ItemFactory::create_steel_sword(void)
{
	return (new SteelSword());
}

// MODIFIED: create_healing_potion() was not implemented
HealingPotion *ItemFactory::create_healing_potion(void)
{
	return (new HealingPotion());
}

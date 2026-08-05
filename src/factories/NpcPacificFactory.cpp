#include "factories/NpcPacificFactory.hpp"

#include <stdexcept>
#include <map>

#include "utils/utils.hpp"
#include "characters/Enchanter.hpp"
#include "characters/Merchant.hpp"
#include "characters/Narrator.hpp"
#include "characters/QuestGiver.hpp"
#include "enchantments/Enchantment.hpp"
#include "enchantments/Flame.hpp"
#include "enchantments/Freeze.hpp"
#include "factories/EnchantmentFactory.hpp"
#include "factories/ItemFactory.hpp"
#include "items/consumables/Apple.hpp"
#include "items/consumables/FirePotion.hpp"
#include "items/consumables/IcePotion.hpp"
#include "items/consumables/HastePotion.hpp"
#include "items/consumables/HealingPotion.hpp"
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
#include "items/armor/BronzeArmor.hpp"
#include "items/armor/EmberArmor.hpp"
#include "items/armor/FrostArmor.hpp"
#include "items/armor/IronArmor.hpp"
#include "items/armor/SpikeArmor.hpp"
#include "items/armor/SteelArmor.hpp"


// Specific enemies -----------------------------------------------------------

// Enchanter --

Enchanter	*NpcPacificFactory::create_enchanter_basic(const std::string& name, const std::string& description)
{
	std::map<Enchantment*, unsigned int>	pool;

	pool[EnchantmentFactory::create_flame()] = 50;
	return (new Enchanter(name, description, pool));
}

Enchanter	*NpcPacificFactory::create_enchanter_advanced(const std::string& name, const std::string& description)
{
	std::map<Enchantment*, unsigned int>	pool;

	pool[EnchantmentFactory::create_freeze()] = 80;
	return (new Enchanter(name, description, pool));
}

// Merchant --

Merchant	*NpcPacificFactory::create_merchant_basic(const std::string& name, const std::string& description)
{
	std::map<Item*, unsigned int>	pool;

	pool[ItemFactory::create_apple()] = 5;
	pool[ItemFactory::create_healing_potion()] = 10;
	pool[ItemFactory::create_poison_potion()] = 12;

	pool[ItemFactory::create_bronze_armor()] = 20;
	pool[ItemFactory::create_steel_armor()] = 30;
	pool[ItemFactory::create_iron_armor()] = 40;
	pool[ItemFactory::create_bronze_armor()] = 30;

	pool[ItemFactory::create_bronze_shield()] = 15;
	pool[ItemFactory::create_iron_shield()] = 15;

	pool[ItemFactory::create_bronze_sword()] = 18;
	pool[ItemFactory::create_iron_sword()] = 18;
	pool[ItemFactory::create_steel_sword()] = 18;



	return (new Merchant(name, description, pool));
}

Merchant	*NpcPacificFactory::create_merchant_advanced(const std::string& name, const std::string& description)
{
	std::map<Item*, unsigned int>	pool;

	pool[ItemFactory::create_apple()] = 5;
	pool[ItemFactory::create_fire_potion()] = 12;
	pool[ItemFactory::create_ice_potion()] = 12;
	pool[ItemFactory::create_haste_potion()] = 12;

	pool[ItemFactory::create_ember_armor()] = 30;
	pool[ItemFactory::create_frost_armor()] = 30;
	pool[ItemFactory::create_spike_armor()] = 30;

	pool[ItemFactory::create_spike_shield()] = 16;
	pool[ItemFactory::create_steel_shield()] = 16;

	pool[ItemFactory::create_flame_sword()] = 18;
	pool[ItemFactory::create_frost_sword()] = 18;
	pool[ItemFactory::create_serrated_sword()] = 18;



	return (new Merchant(name, description, pool));
}

// Narrator --

Narrator	*NpcPacificFactory::create_narrator(const std::string& name, const std::string& description, const std::string& sentence)
{
	return (new Narrator(name, description, sentence));
}

// Quest giver --

QuestGiver	*NpcPacificFactory::create_quest_giver(const std::string& name, const std::string& description, Quest& quest, t_quest_dialogues dialogues)
{
	return (new QuestGiver(name, description, quest, dialogues));
}

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
#include "items/armor/BronzeArmor.hpp"

// Specific enemies -----------------------------------------------------------

// Enchanter --

Enchanter	*NpcPacificFactory::create_enchanter_basic(const std::string& name, const std::string& description) noexcept
{
	std::map<Enchantment*, unsigned int>	pool;

	// TODO: Other enchantments
	pool[EnchantmentFactory::create_flame()] = 50;
	pool[EnchantmentFactory::create_freeze()] = 80;
	return (new Enchanter(name, description, pool));
}

Enchanter	*NpcPacificFactory::create_enchanter_advanced(const std::string& name, const std::string& description) noexcept
{
	std::map<Enchantment*, unsigned int>	pool;

	// TODO: Other enchantments
	pool[EnchantmentFactory::create_flame()] = 50;
	pool[EnchantmentFactory::create_freeze()] = 80;
	return (new Enchanter(name, description, pool));
}

// Merchant --

Merchant	*NpcPacificFactory::create_merchant_basic(const std::string& name, const std::string& description) noexcept
{
	std::map<Item*, unsigned int>	pool;

	// TODO: Other items
	pool[ItemFactory::create_apple()] = 5;
	pool[ItemFactory::create_bronze_armor()] = 30;
	return (new Merchant(name, description, pool));
}

Merchant	*NpcPacificFactory::create_merchant_advanced(const std::string& name, const std::string& description) noexcept
{
	std::map<Item*, unsigned int>	pool;

	// TODO: Other items
	pool[ItemFactory::create_apple()] = 5;
	pool[ItemFactory::create_bronze_armor()] = 30;
	return (new Merchant(name, description, pool));
}

// Narrator --

Narrator	*NpcPacificFactory::create_narrator(const std::string& name, const std::string& description, const std::string& sentence) noexcept
{
	return (new Narrator(name, description, sentence));
}

// Quest giver --

QuestGiver	*NpcPacificFactory::create_quest_giver(const std::string& name, const std::string& description, Quest& quest, t_quest_dialogues dialogues) noexcept
{
	return (new QuestGiver(name, description, quest, dialogues));
}

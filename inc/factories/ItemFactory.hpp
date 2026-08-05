#pragma once
#include <map>
#include <string>

#include "utils/attributes.hpp"

class Item;
class Apple;
class BronzeArmor;
class BronzeShield;
class BronzeSword;
class ChestKey;
class EmberArmor;
class FirePotion;
class FlameSword;
class FrostArmor;
class FrostSword;
class HastePotion;
class IcePotion;
class IronArmor;
class IronShield;
class IronSword;
class PoisonPotion;
class SerratedSword;
class SpikeArmor;
class SpikeShield;
class SteelArmor;
class SteelShield;
class SteelSword;
class HealingPotion;

class ItemFactory
{
	public:
		/**
		 * @brief	Creates a default pool with a chance related to each Item.
		 * @note	This function is mainly used for the chest pool generation.
		 */
		static std::map<Item*, unsigned int>	create_default_pool(void) TAP_COLD;
		
		/**
		 * @brief	Creates the item the string refers to.
		 * @param	item_name	The name of the item to create.
		 * @returns	An instance of the created item.
		 * @throws	`std::invalid_argument` if `item_name` does not match
		 * 			any item of the game.
		 * @note	This method is mainly used in the parser.
		 */
		static Item								*create_from_name(const std::string& item_name) TAP_RETURNS_NONNULL TAP_UNUSED_RESULT;

		// Specific items -----------------------------------------------------

		static Apple			*create_apple(void) TAP_RETURNS_NONNULL TAP_UNUSED_RESULT;
		static BronzeArmor		*create_bronze_armor(void) TAP_RETURNS_NONNULL TAP_UNUSED_RESULT;
		static BronzeShield		*create_bronze_shield(void) TAP_RETURNS_NONNULL TAP_UNUSED_RESULT;
		static BronzeSword		*create_bronze_sword(void) TAP_RETURNS_NONNULL TAP_UNUSED_RESULT;
		static ChestKey			*create_chest_key(void) TAP_RETURNS_NONNULL TAP_UNUSED_RESULT;
		static EmberArmor		*create_ember_armor(void) TAP_RETURNS_NONNULL TAP_UNUSED_RESULT;
		static FirePotion		*create_fire_potion(void) TAP_RETURNS_NONNULL TAP_UNUSED_RESULT;
		static FlameSword		*create_flame_sword(void) TAP_RETURNS_NONNULL TAP_UNUSED_RESULT;
		static FrostArmor		*create_frost_armor(void) TAP_RETURNS_NONNULL TAP_UNUSED_RESULT;
		static FrostSword		*create_frost_sword(void) TAP_RETURNS_NONNULL TAP_UNUSED_RESULT;
		static HastePotion		*create_haste_potion(void) TAP_RETURNS_NONNULL TAP_UNUSED_RESULT;
		static IcePotion		*create_ice_potion(void) TAP_RETURNS_NONNULL TAP_UNUSED_RESULT;
		static IronArmor		*create_iron_armor(void) TAP_RETURNS_NONNULL TAP_UNUSED_RESULT;
		static IronShield		*create_iron_shield(void) TAP_RETURNS_NONNULL TAP_UNUSED_RESULT;
		static IronSword		*create_iron_sword(void) TAP_RETURNS_NONNULL TAP_UNUSED_RESULT;
		static PoisonPotion		*create_poison_potion(void) TAP_RETURNS_NONNULL TAP_UNUSED_RESULT;
		static SerratedSword	*create_serrated_sword(void) TAP_RETURNS_NONNULL TAP_UNUSED_RESULT;
		static SpikeArmor		*create_spike_armor(void) TAP_RETURNS_NONNULL TAP_UNUSED_RESULT;
		static SpikeShield		*create_spike_shield(void) TAP_RETURNS_NONNULL TAP_UNUSED_RESULT;
		static SteelArmor		*create_steel_armor(void) TAP_RETURNS_NONNULL TAP_UNUSED_RESULT;
		static SteelShield		*create_steel_shield(void) TAP_RETURNS_NONNULL TAP_UNUSED_RESULT;
		static SteelSword		*create_steel_sword(void) TAP_RETURNS_NONNULL TAP_UNUSED_RESULT;
		static HealingPotion	*create_healing_potion(void) TAP_RETURNS_NONNULL TAP_UNUSED_RESULT;
};

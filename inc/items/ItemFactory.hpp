#pragma once
#include <map>
#include <string>

#include "utils/attributes.hpp"

class Item;
class ChestKey;
class IronArmor;
class IronShield;
class IronSword;

class ItemFactory
{
	public:
		/**
		 * @brief	Creates a default pool with a chance related to each Item.
		 * @note	This function is mainly used for the chest pool generation.
		 */
		static std::map<Item*, unsigned int>	create_default_pool(void) noexcept TAP_COLD;
		
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

		static ChestKey		*create_chest_key(void) noexcept TAP_RETURNS_NONNULL TAP_UNUSED_RESULT;
		static IronArmor	*create_iron_armor(void) noexcept TAP_RETURNS_NONNULL TAP_UNUSED_RESULT;
		static IronShield	*create_iron_shield(void) noexcept TAP_RETURNS_NONNULL TAP_UNUSED_RESULT;
		static IronSword	*create_iron_sword(void) noexcept TAP_RETURNS_NONNULL TAP_UNUSED_RESULT;
};

#pragma once
#include <map>

class Item;
class IronArmor;
class IronShield;
class IronSword;

class ItemFactory
{
	public:
		/**
		 * @brief	Creates a default pool with a chance related to each Item.
		 */
		static std::map<Item*, unsigned int>	create_default_pool(void) noexcept;
		static IronArmor						*create_iron_armor(void) noexcept;
		static IronShield						*create_iron_shield(void) noexcept;
		static IronSword						*create_iron_sword(void) noexcept;
};

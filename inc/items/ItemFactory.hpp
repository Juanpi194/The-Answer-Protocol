#pragma once
#include <map>

class Item;
class IronSword;

class ItemFactory
{
	public:
		/**
		 * @brief	Creates a default pool with a chance related to each Item.
		 * 			The sum of the numbers must be 100.
		 */
		static std::map<Item*, unsigned int>	create_default_pool(void);
		static IronSword						*create_iron_sword(void);
};

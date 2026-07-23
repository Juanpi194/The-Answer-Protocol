#pragma once
#include <list>
#include <string>

#include "utils/attributes.hpp"
#include "utils/types.hpp"

class Item;

class Inventory
{
	private:
		std::list<Item*>	items;
	public:
		// Constructors -------------------------------------------------------

		Inventory(void) = default;
		Inventory(const Inventory& inventory) = delete;
		~Inventory(void);

		// Operators ----------------------------------------------------------

		Inventory&	operator=(const Inventory& other) = delete;

		// Getters and setters ------------------------------------------------

		const std::list<Item*>&	get_items(void) const noexcept;

		// Utils --------------------------------------------------------------

		/**
		 * @brief	Simply adds an item to the player's item list,
		 * 			without any checking.
		 * @param	item	The item to be added.
		 * @note	Be careful when using this method.
		 */
		void	add_item(Item *item) noexcept TAP_NONNULL;

		/**
		 * @brief	Simply removes an item from the player's item list,
		 * 			without any checking.
		 * @param	item	The item to be removed.
		 * @note	Be careful when using this method.
		 */
		void	remove_item(Item *item) noexcept TAP_NONNULL;

		/**
		 * @brief	Removes an item from the player's item list, removing it
		 * 			from the list and deleting it.
		 * @param	item	The item to delete.
		 * @throws	`std::invalid_argument` if the item is not in the player's
		 * 			item list.
		 */
		void	consume_item(Item& item);

		/**
		 * @brief	Finds an item from the fighter's item list that matches the
		 * 			specified type.
		 * @returns	A pointer to the first instance found of that type,
		 * 			`nullptr` if no item of the specified type in the list.
		 * @note	It is defined in the .hpp because it uses templates.
		 */
		template<typename T>
		T		*find_item(void)
		{
			for (Item *item_in_list: items)
			{
				if (is_instance<T>(item_in_list))
					return (dynamic_cast<T*>(item_in_list));
			}
			return (nullptr);
		}

		/**
		 * @brief	Tries to find an item in the player's item list that
		 * 			matches the specified name.
		 * @param	item_name	The name of the item to search.
		 * @returns	The item found with that name, `nullptr` if no item
		 * 			was found with the specified name.
		 */
		Item	*find_item_by_name(const std::string& item_name) const TAP_UNUSED_RESULT;

};

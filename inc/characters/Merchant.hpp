#pragma once
#include <map>

#include "characters/NPC.hpp"

class Item;

constexpr unsigned int	MAX_ITEM_PRICE = 2000;

class Merchant final: public NPC
{
	private:
		/**
		 * @brief	Item list that the merchant will be selling.
		 * 			Each item has its own prize. Const so it never
		 * 			changes. Buying an item makes a copy of it,
		 * 			the merchant will still have it to keep selling it.
		 */
		const std::map<Item*, unsigned int>	items_to_sell;

		/**
		 * @brief	Creates a string with all the shop items and
		 * 			their respective prices.
		 * @returns	All items with a string format.
		 */
		std::string	items_to_sell_str_format(void) const noexcept;
	public:
		// Constructors -------------------------------------------------------

		/**
		 * @throws	`std::invalid_argument` if there is any `nullptr` in the `map`.
		 */
		Merchant(const std::string& id, const std::string& name, const std::string& description, const std::map<Item*, unsigned int>& items_to_sell);
		Merchant(const Merchant& merchant) = delete;

		/**
		 * @brief	Frees all items from the dictionary.
		 */
		~Merchant(void);

		// Operators ----------------------------------------------------------

		Merchant&	operator=(const Merchant& other) = delete;

		// Getters and setters ------------------------------------------------

		const std::map<Item*, unsigned int>&	get_items_to_sell(void) const noexcept;

		// Utils --------------------------------------------------------------

		void	on_interact(Player& player) override;
};

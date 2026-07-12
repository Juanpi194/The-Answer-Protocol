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

		static unsigned int			available_id;	// Defined in Merchant.cpp
		static const std::string	PREFIX;	// Defined in Merchant.cpp
	public:
		// Constructors -------------------------------------------------------

		/**
		 * @throws	`std::invalid_argument` if there is any `nullptr` in the `map`.
		 */
		Merchant(const std::string& name, const std::string& description, const std::map<Item*, unsigned int>& items_to_sell);

		/**
		 * @note	It is not allowed to make a copy because each merchant should be unique.
		 */
		Merchant(const Merchant& merchant) = delete;

		/**
		 * @brief	Frees all items from the map.
		 */
		~Merchant(void);

		// Operators ----------------------------------------------------------

		Merchant&	operator=(const Merchant& other) = delete;

		// Getters and setters ------------------------------------------------

		const std::map<Item*, unsigned int>&	get_items_to_sell(void) const noexcept;

		// Utils --------------------------------------------------------------

		void	on_interact(Player& player) override;
};

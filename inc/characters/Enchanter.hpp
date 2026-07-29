#pragma once
#include <map>

#include "characters/NPC.hpp"
#include "characters/Vendor.hpp"
#include "utils/attributes.hpp"

class Gear;
class Enchantment;

class Enchanter final: public NPC, public Vendor
{
	private:
		/**
		 * @brief	Enchantment list that the merchant will be selling.
		 * 			Each enchantment has its own prize. Const so it never
		 * 			changes. Buying an enchantment makes a copy of it,
		 * 			the enchanter will still have it to keep selling it.
		 */
		const std::map<Enchantment*, unsigned int>	enchantments_to_sell;

		static unsigned int			available_id;	// Defined in Enchanter.cpp
		static const std::string	PREFIX;	// Definied in Enchanter.cpp
		
		/**
		 * @brief	Applies the specified enchantment to a gear item if the item can
		 * 			get the provided enchantment.
		 * @param	gear	The gear item to enchant.
		 * @param	enchantment	Pointer to the enchant to be set to the item.
		 * @throws	`std::invalid_argument` if `enchantment` is `nullptr`.
		 */
		bool	enchant(Gear& gear, Enchantment& enchantment) TAP_UNUSED_RESULT;
	public:
		// Constructors -------------------------------------------------------

		/**
		 * @throws	`std::invalid_argument` if there is any `nullptr` in the `map`.
		 */
		Enchanter(const std::string& name, const std::string& description, const std::map<Enchantment*, unsigned int>& enchantments_to_sell);

		/**
		 * @note	It is not allowed to make a copy because each enchanter should be unique.
		 */
		Enchanter(const Enchanter& enchanter) = delete;

		/**
		 * @brief	Frees all Enchantments from the map.
		 */
		~Enchanter(void);

		// Operators ----------------------------------------------------------

		Enchanter&	operator=(const Enchanter& other) = delete;

		// Getters and setters ------------------------------------------------

		const std::map<Enchantment*, unsigned int>&	get_enchantments_to_sell(void) const noexcept;

		// Utils --------------------------------------------------------------
		// TODO: Throws exception if enchantment doesn't work for gear.

		void	on_talk(Player& player) noexcept override;

		/**
		 * @brief	Tries to enchant the specified gear with the specified
		 * 			enchantment. If the gear is not found, the specified item
		 * 			is not a gear or the enchantment is not found, nothing
		 * 			will be done.
		 * @param	player	The player that tries to enchant the gear.
		 * @param	gear	The gear being searched to apply the enchantment.
		 * @param	enchantment	The enchantment searched to apply to `gear`.
		 */
		void	on_enchant(Player& player, const std::string& gear, const std::string& enchantment);

		void	on_buy(Player& player, const std::string& product) noexcept override;
};

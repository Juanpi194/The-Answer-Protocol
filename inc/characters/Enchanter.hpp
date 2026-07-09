#pragma once
#include <map>

#include "characters/NPC.hpp"

class Gear;
class Enchantment;

class Enchanter final: public NPC
{
	private:
		/**
		 * @brief	Enchantment list that the merchant will be selling.
		 * 			Each enchantment has its own prize. Const so it never
		 * 			changes. Buying an enchantment makes a copy of it,
		 * 			the enchanter will still have it to keep selling it.
		 */
		const std::map<Enchantment*, unsigned int>	enchantments;
	public:
		// Constructors -------------------------------------------------------

		/**
		 * @throws	`std::invalid_argument` if there is any `nullptr` in the `map`.
		 */
		Enchanter(const std::string& name, const std::string& description, const std::map<Enchantment*, unsigned int>& enchantments);

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

		const std::map<Enchantment*, unsigned int>&	get_enchantments(void) const noexcept;

		// Utils --------------------------------------------------------------
		// TODO: Throws exception if enchantment doesn't work for gear.

		void	on_interact(Player& player) override;

		/**
		 * @brief	Applies the specified enchantment to a gear item.
		 * @param	gear	The gear item to enchant.
		 * @param	enchantment	Pointer to the enchant to be set to the item.
		 * @throws	`std::invalid_argument` if `enchantment` is `nullptr`.
		 */
		void	enchant(Gear& gear, Enchantment *enchantment) __nonnull();
};

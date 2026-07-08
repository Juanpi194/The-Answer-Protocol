#pragma once
#include <map>

#include "characters/NPC.hpp"

class Gear;
class Enchantment;

class Enchanter final: public NPC
{
	private:
		const std::map<Enchantment*, unsigned int>	enchantments;	// Enchantment and price
	public:
		// Constructors -------------------------------------------------------

		// TODO: Add enchantments to the constructor
		Enchanter(const std::string& name, const std::string& description, const std::map<Enchantment*, unsigned int> enchantments);
		Enchanter(const Enchanter& enchanter) = delete;
		~Enchanter(void);

		// Operators ----------------------------------------------------------

		Enchanter&	operator=(const Enchanter& other) = delete;

		// Getters and setters ------------------------------------------------

		const std::map<Enchantment*, unsigned int>	get_enchantments(void) const noexcept;

		// Utils --------------------------------------------------------------
		// TODO: Throws exception if enchantment doesn't work for gear.

		void	on_interact(Player& player) override;

		/**
		 * @brief	Applies the specified enchantment to a gear item.
		 * @param	gear	The gear item to enchant.
		 * @param	enchantment	The enchant to be set to the item.
		 */
		void	enchant(Gear& gear, Enchantment enchantment) noexcept;
};

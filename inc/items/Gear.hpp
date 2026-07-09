#pragma once

#include "items/Item.hpp"
#include "utils/types.hpp"

class Enchantment;

/**
 * @brief	Abstract class that represents equipment (Weapons and Armor).
 */
class Gear: public Item
{
	private:
		/**
		 * @brief	Used for enchantments.
		 */
		const GearType	gear_type;
		/**
		 * @brief	The enchantment that the gear item has.
		 * @note	Only one enchantment per item.
		 */
		Enchantment	*enchantment;
	protected:
		/**
		 * @brief	This method will be called by `use` by each type of gear.
		 */
		virtual void	special_use(void) noexcept = 0;
	public:
		// Operators ----------------------------------------------------------

		Gear(const std::string& name, const std::string& description, GearType gear_type);
		/**
		 * @note	The copy will not have the enchantments from the original.
		 */
		Gear(const Gear& gear);
		virtual ~Gear(void);

		// Operators ----------------------------------------------------------

		Gear&	operator=(const Gear& other) = delete;

		// Getters and setters ------------------------------------------------

		GearType	get_gear_type(void) const noexcept;
		Enchantment	*get_enchantment(void) const noexcept;

		void	set_enchantment(Enchantment *enchantment);

		// Utils --------------------------------------------------------------

		// TODO: Add utils functions. Do not override the use method.
		// ? REVIEW: If the use method is going to have same logic in Weapon and in Armor
		// ? 		 move the method to Gear class.
};

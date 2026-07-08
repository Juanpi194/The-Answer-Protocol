#pragma once

#include "items/Item.hpp"

class Gear: public Item
{
	private:
		// TODO: Add attributes
	protected:
		// TODO: Enchantments...

		/**
		 * @brief	This method will be called by `use` by each type of gear.
		 */
		virtual void	special_use(void) noexcept = 0;
	public:
		// Operators ----------------------------------------------------------

		Gear(const std::string& id, const std::string& name, const std::string& description);
		Gear(const Gear& gear) = delete;
		~Gear(void) = default;

		// Operators ----------------------------------------------------------

		Gear&	operator=(const Gear& other) = delete;

		// Getters and setters ------------------------------------------------

		// TODO: Add getters and setters for the attributes

		// Utils --------------------------------------------------------------

		// TODO: Add utils functions. Do not override the use method.
		// ? REVIEW: If the use method is going to have same logic in Weapon and in Armor
		// ? 		 move the method to Gear class.
};

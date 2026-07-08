#pragma once
#include <string>
#include <list>

#include "utils/types.hpp"

class Gear;

class Enchantment
{
	private:
		std::string					name;
		std::string					description;
		/**
		 * @brief	Each enchantment will only be able to be set to a gear
		 * 			that is allowed.
		 */
		const std::list<GearType>	allowed_gear_types;

		static constexpr size_t	MIN_NAME_LENGTH = 3;
		static constexpr size_t	MAX_NAME_LENGTH = 20;
		static constexpr size_t	MIN_DESCRIPTION_LENGTH = 10;
		static constexpr size_t	MAX_DESCRIPTION_LENGTH = 80;

		/**
		 * @brief	Checks that all arguments for the enchantment initialization are valid.
		 * @returns	`true` if passes the validation process. `false` otherwise.
		 * @note	This method should be used ONLY in the constructor.
		 */
		bool	validate_arguments(const std::string& name, const std::string& description);
	
		/**
		 * @brief	Checks if the given gear could get the enchantment.
		 * @param	gear	The gear to set the enchantment to.
		 * @returns	`true` if the enchantment can be set. `false` otherwise.
		 */
		bool	can_apply(Gear& gear);
	public:
		// Constructors -------------------------------------------------------

		/**
		 * @throws	`std::invalid_argument` if validation fails.
		 */
		Enchantment(const std::string& name, const std::string& description, const std::list<GearType> allowed_gear_types);
		Enchantment(const Enchantment& enchantment);
		virtual ~Enchantment(void) = default;

		// Operators ----------------------------------------------------------

		Enchantment&	operator=(const Enchantment& other) = delete;

		// Getters and setters ------------------------------------------------

		std::string					get_name(void) const noexcept;
		std::string					get_description(void) const noexcept;
		const std::list<GearType>	get_allowed_gear_types(void) const noexcept;

		// Utils --------------------------------------------------------------

		/**
		 * @brief	Tries to apply the enchantment to the provided gear.
		 * @param	gear	The gear to enchant.
		 */
		void			apply_to(Gear& gear);
		virtual void	effect(void) noexcept = 0;
};

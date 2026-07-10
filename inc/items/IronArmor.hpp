#pragma once

#include "items/Armor.hpp"

class IronArmor: public Armor
{
	private:
		// TODO: Add attributes

		static const std::string		NAME;	// Defined in IronArmor.cpp
		static const std::string		DESCRIPTION;	// Defined in IronArmor.cpp
		static constexpr unsigned int	DAMAGE_REDUCTION = 3;
		static constexpr GearType		GEAR_TYPE = GearType::Iron;

		void	special_use(void) noexcept override;
	public:
		// Constructors -------------------------------------------------------

		IronArmor(void);
		IronArmor(const IronArmor& iron_armor);
		~IronArmor(void) = default;

		// Operators ----------------------------------------------------------

		IronArmor&	operator=(const IronArmor& other) = delete;

		// Getters and setters ------------------------------------------------

		// TODO: Add getters and setters for future attributes.

		// Utils --------------------------------------------------------------
};

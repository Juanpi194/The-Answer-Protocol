#pragma once

#include "items/Armor.hpp"

class IronArmor final: public Armor
{
	private:
		static unsigned int				available_id;	// Defined in IronArmor.cpp
		static const std::string		PREFIX;	// Defined in IronArmor.cpp
		static constexpr GearType		GEAR_TYPE = GearType::Iron;

		void	special_use(void) noexcept override;
	public:
		static const std::string		NAME;	// Defined in IronArmor.cpp
		static const std::string		DESCRIPTION;	// Defined in IronArmor.cpp
		static constexpr unsigned int	DAMAGE_REDUCTION = 3;

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

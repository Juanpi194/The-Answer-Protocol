#pragma once

#include "items/Armor.hpp"

class BronzeArmor final: public Armor
{
	private:
		static unsigned int				available_id;	// Defined in BronzeArmor.cpp
		static const std::string		PREFIX;	// Defined in BronzeArmor.cpp
		static constexpr GearType		GEAR_TYPE = GearType::IRON;

		// void	special_use(void) noexcept override;
	public:
		static const std::string		NAME;	// Defined in BronzeArmor.cpp
		static const std::string		DESCRIPTION;	// Defined in BronzeArmor.cpp
		static constexpr unsigned int	DAMAGE_REDUCTION = 3;

		// Constructors -------------------------------------------------------

		BronzeArmor(void);
		BronzeArmor(const BronzeArmor& bronze_armor);
		~BronzeArmor(void) = default;
		BronzeArmor	*clone(void) const noexcept override TAP_RETURNS_NONNULL;

		// Operators ----------------------------------------------------------

		BronzeArmor&	operator=(const BronzeArmor& other) = delete;

		// Getters and setters ------------------------------------------------

		// TODO: Add getters and setters for future attributes.

		// Utils --------------------------------------------------------------
};

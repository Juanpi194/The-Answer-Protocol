#pragma once

#include "items/Armor.hpp"

class SteelArmor final: public Armor
{
	private:
		static unsigned int				available_id;	// Defined in SteelArmor.cpp
		static const std::string		PREFIX;	// Defined in SteelArmor.cpp
		static constexpr GearType		GEAR_TYPE = GearType::IRON;

		// void	special_use(void) noexcept override;
	public:
		static const std::string		NAME;	// Defined in SteelArmor.cpp
		static const std::string		DESCRIPTION;	// Defined in SteelArmor.cpp
		static constexpr unsigned int	DAMAGE_REDUCTION = 5;

		// Constructors -------------------------------------------------------

		SteelArmor(void);
		SteelArmor(const SteelArmor& steel_armor);
		~SteelArmor(void) = default;
		SteelArmor	*clone(void) const noexcept override TAP_RETURNS_NONNULL;

		// Operators ----------------------------------------------------------

		SteelArmor&	operator=(const SteelArmor& other) = delete;

};

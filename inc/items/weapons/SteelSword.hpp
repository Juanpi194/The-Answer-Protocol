#pragma once

#include "items/Weapon.hpp"

class SteelSword final: public Weapon
{
	private:
		static unsigned int				available_id;	// Defined in SteelSword.cpp
		static const std::string		PREFIX;	// Defined in SteelSword.cpp
		static constexpr GearType		GEAR_TYPE = GearType::IRON;

		// void	special_use(void) noexcept override;
	public:
		static const std::string		NAME;	// Defined in SteelSword.cpp
		static const std::string		DESCRIPTION;	// Defined in SteelSword.cpp
		static constexpr unsigned int	EXTRA_DAMAGE = 4;

		// Constructors -------------------------------------------------------

		SteelSword(void);
		SteelSword(const SteelSword& steel_sword);
		~SteelSword(void) = default;
		SteelSword	*clone(void) const noexcept override TAP_RETURNS_NONNULL;

		// Operators ----------------------------------------------------------

		SteelSword&	operator=(const SteelSword& other) = delete;

		// Getters and setters ------------------------------------------------

		// TODO: Add getters and setters for future attributes.

		// Utils --------------------------------------------------------------
};

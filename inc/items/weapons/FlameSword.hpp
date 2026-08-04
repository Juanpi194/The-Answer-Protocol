#pragma once

#include "characters/Fighter.hpp"
#include "items/SpecialEffectGear.hpp"
#include "items/Weapon.hpp"

class FlameSword final: public Weapon, public SpecialEffectGear
{
	private:
		static unsigned int				available_id;	// Defined in FlameSword.cpp
		static const std::string		PREFIX;	// Defined in FlameSword.cpp
		static constexpr GearType		GEAR_TYPE = GearType::FIRE;
	public:
		static const std::string		NAME;	// Defined in FlameSword.cpp
		static const std::string		DESCRIPTION;	// Defined in FlameSword.cpp
		static constexpr unsigned int	EXTRA_DAMAGE = 5;
		static constexpr Status			APPLIED_STATUS = Status::BURNT;

		// Constructors -------------------------------------------------------

		FlameSword(void);
		FlameSword(const FlameSword& flame_sword);
		~FlameSword(void) = default;
		FlameSword	*clone(void) const noexcept override TAP_RETURNS_NONNULL;

		// Operators ----------------------------------------------------------

		FlameSword&	operator=(const FlameSword& other) = delete;

		// Getters and setters ------------------------------------------------

		// TODO: Add getters and setters for future attributes.

		// Utils --------------------------------------------------

		void	special_effect(Fighter& user, Fighter& opponent) noexcept override;
};

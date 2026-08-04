#pragma once

#include "characters/Fighter.hpp"
#include "items/SpecialEffectGear.hpp"
#include "items/Weapon.hpp"

class FrostSword final: public Weapon, public SpecialEffectGear
{
	private:
		static unsigned int				available_id;	// Defined in FrostSword.cpp
		static const std::string		PREFIX;	// Defined in FrostSword.cpp
		static constexpr GearType		GEAR_TYPE = GearType::ICE;
	public:
		static const std::string		NAME;	// Defined in FrostSword.cpp
		static const std::string		DESCRIPTION;	// Defined in FrostSword.cpp
		static constexpr unsigned int	EXTRA_DAMAGE = 7;
		static constexpr Status			APPLIED_STATUS = Status::FROZEN;

		// Constructors -------------------------------------------------------

		FrostSword(void);
		FrostSword(const FrostSword& frost_sword);
		~FrostSword(void) = default;
		FrostSword	*clone(void) const noexcept override TAP_RETURNS_NONNULL;

		// Operators ----------------------------------------------------------

		FrostSword&	operator=(const FrostSword& other) = delete;

		// Utils --------------------------------------------------

		void	special_effect(Fighter& user, Fighter& opponent) noexcept override;
};

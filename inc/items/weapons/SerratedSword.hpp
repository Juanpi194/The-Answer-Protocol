#pragma once

#include "items/SpecialEffectGear.hpp"
#include "items/Weapon.hpp"

class SerratedSword final: public Weapon, public SpecialEffectGear
{
	private:
		static unsigned int				available_id;	// Defined in SerratedSword.cpp
		static const std::string		PREFIX;	// Defined in SerratedSword.cpp
		static constexpr GearType		GEAR_TYPE = GearType::IRON;
	public:
		static const std::string		NAME;	// Defined in SerratedSword.cpp
		static const std::string		DESCRIPTION;	// Defined in SerratedSword.cpp
		static constexpr unsigned int	EXTRA_DAMAGE = 6;

		// Constructors -------------------------------------------------------

		SerratedSword(void);
		SerratedSword(const SerratedSword& serrated_sword);
		~SerratedSword(void) = default;
		SerratedSword	*clone(void) const noexcept override TAP_RETURNS_NONNULL;

		// Operators ----------------------------------------------------------

		SerratedSword&	operator=(const SerratedSword& other) = delete;

		// Getters and setters ------------------------------------------------

		// TODO: Add getters and setters for future attributes.

		// Utils --------------------------------------------------

		void	special_effect(Fighter& user, Fighter& opponent) noexcept override;
};

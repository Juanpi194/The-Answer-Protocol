#pragma once

#include "items/Armor.hpp"
#include "items/SpecialEffectGear.hpp"

class FrostArmor final: public Armor, public SpecialEffectGear
{
	private:
		static unsigned int				available_id;	// Defined in FrostArmor.cpp
		static const std::string		PREFIX;	// Defined in FrostArmor.cpp
		static constexpr GearType		GEAR_TYPE = GearType::ICE;

		/**
		 * @brief	Duration of the freeze effect applied to the attacker.
		 */
		static constexpr unsigned int	FREEZE_DURATION = 2;

	public:
		static const std::string		NAME;	// Defined in FrostArmor.cpp
		static const std::string		DESCRIPTION;	// Defined in FrostArmor.cpp
		static constexpr unsigned int	DAMAGE_REDUCTION = 2;

		// Constructors -------------------------------------------------------

		FrostArmor(void);
		FrostArmor(const FrostArmor& frost_armor);
		~FrostArmor(void) = default;
		FrostArmor	*clone(void) const noexcept override TAP_RETURNS_NONNULL;

		// Operators ----------------------------------------------------------

		FrostArmor&	operator=(const FrostArmor& other) = delete;

		// Getters and setters ------------------------------------------------

		// TODO: Add getters and setters for future attributes.

		// Utils --------------------------------------------------------------

		void	special_effect(Fighter& user, Fighter& opponent) noexcept override;
};

#pragma once

#include "characters/Fighter.hpp"
#include "items/Armor.hpp"
#include "items/SpecialEffectGear.hpp"

class EmberArmor final: public Armor, public SpecialEffectGear
{
	private:
		static unsigned int				available_id;	// Defined in EmberArmor.cpp
		static const std::string		PREFIX;	// Defined in EmberArmor.cpp
		static constexpr GearType		GEAR_TYPE = GearType::FIRE;

		/**
		 * @brief	Duration of the burn effect applied to the attacker.
		 */
		// static constexpr unsigned int	BURN_DURATION = 2;

	public:
		static const std::string		NAME;	// Defined in EmberArmor.cpp
		static const std::string		DESCRIPTION;	// Defined in EmberArmor.cpp
		static constexpr unsigned int	DAMAGE_REDUCTION = 4;
		static constexpr Status			APPLIED_STATUS = Status::BURNT;

		// Constructors -------------------------------------------------------

		EmberArmor(void);
		EmberArmor(const EmberArmor& ember_armor);
		~EmberArmor(void) = default;
		EmberArmor	*clone(void) const noexcept override TAP_RETURNS_NONNULL;

		// Operators ----------------------------------------------------------

		EmberArmor&	operator=(const EmberArmor& other) = delete;

		// Getters and setters ------------------------------------------------

		// TODO: Add getters and setters for future attributes.

		// Utils --------------------------------------------------------------

		void	special_effect(Fighter& user, Fighter& opponent) noexcept override;
};

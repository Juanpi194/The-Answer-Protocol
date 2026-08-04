#pragma once

#include "characters/Fighter.hpp"
#include "items/Armor.hpp"
#include "items/SpecialEffectGear.hpp"

class SpikeArmor final: public Armor, public SpecialEffectGear
{
	private:
		static unsigned int				available_id;	// Defined in SpikeArmor.cpp
		static const std::string		PREFIX;	// Defined in SpikeArmor.cpp
		static constexpr GearType		GEAR_TYPE = GearType::IRON;

		/**
		 * @brief	Damage dealt by the armor to the attacker.
		 */
		static constexpr unsigned int	DAMAGE_IN_CONTACT = 3;

	public:
		static const std::string		NAME;	// Defined in SpikeArmor.cpp
		static const std::string		DESCRIPTION;	// Defined in SpikeArmor.cpp
		static constexpr unsigned int	DAMAGE_REDUCTION = 5;
		static constexpr unsigned int	REFLECT_DAMAGE = 3;
		static constexpr Status			APPLIED_STATUS = Status::POISONED;

		// Constructors -------------------------------------------------------

		SpikeArmor(void);
		SpikeArmor(const SpikeArmor& spike_armor);
		~SpikeArmor(void) = default;
		SpikeArmor	*clone(void) const noexcept override TAP_RETURNS_NONNULL;

		// Operators ----------------------------------------------------------

		SpikeArmor&	operator=(const SpikeArmor& other) = delete;

		// Utils --------------------------------------------------------------

		void	special_effect(Fighter& user, Fighter& opponent) noexcept override;
};

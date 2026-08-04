#pragma once

#include "items/Shield.hpp"
#include "items/OnContactShield.hpp"
#include "characters/Fighter.hpp"

class SpikeShield final: public Shield, public OnContactShield
{
	private:
		static unsigned int				available_id;	// Defined in SpikeShield.cpp
		static const std::string		PREFIX;	// Defined in SpikeShield.cpp
	public:
		static const std::string		NAME;	// Defined in SpikeShield.cpp
		static const std::string		DESCRIPTION;	// Defined in SpikeShield.cpp
		static constexpr unsigned int	MAX_USES = 5;
		static constexpr unsigned int	REFLECT_DAMAGE = 4;
		static constexpr Status			IN_CONTACT_STATUS = Status::POISONED;

		// Constructors -------------------------------------------------------

		SpikeShield(void);
		SpikeShield(const SpikeShield& spike_shield);
		~SpikeShield(void) = default;
		SpikeShield	*clone(void) const noexcept override TAP_RETURNS_NONNULL;

		// Operators ----------------------------------------------------------

		SpikeShield&	operator=(const SpikeShield& other) = delete;

		// Getters and setters ------------------------------------------------

		// TODO: Add getter to static var

		// Utils --------------------------------------------------------------

		void	on_contact(Fighter& user, Fighter& attacker) noexcept override;
};

#pragma once

#include "items/Shield.hpp"

class SteelShield final: public Shield
{
	private:
		static unsigned int				available_id;	// Defined in SteelShield.cpp
		static const std::string		PREFIX;	// Defined in SteelShield.cpp
	public:
		static const std::string		NAME;	// Defined in SteelShield.cpp
		static const std::string		DESCRIPTION;	// Defined in SteelShield.cpp
		static constexpr unsigned int	MAX_USES = 4;

		// Constructors -------------------------------------------------------

		SteelShield(void);
		SteelShield(const SteelShield& steel_shield);
		~SteelShield(void) = default;
		SteelShield	*clone(void) const noexcept override TAP_RETURNS_NONNULL;

		// Operators ----------------------------------------------------------

		SteelShield&	operator=(const SteelShield& other) = delete;

		// Getters and setters ------------------------------------------------

		// TODO: Add getter to static var

		// Utils --------------------------------------------------------------

		// TODO: Funcs
};

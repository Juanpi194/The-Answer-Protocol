#pragma once

#include "items/Shield.hpp"

class BronzeShield final: public Shield
{
	private:
		static unsigned int				available_id;	// Defined in BronzeShield.cpp
		static const std::string		PREFIX;	// Defined in BronzeShield.cpp
	public:
		static const std::string		NAME;	// Defined in BronzeShield.cpp
		static const std::string		DESCRIPTION;	// Defined in BronzeShield.cpp
		static constexpr unsigned int	MAX_USES = 2;

		// Constructors -------------------------------------------------------

		BronzeShield(void);
		BronzeShield(const BronzeShield& bronze_shield);
		~BronzeShield(void) = default;
		BronzeShield	*clone(void) const noexcept override TAP_RETURNS_NONNULL;

		// Operators ----------------------------------------------------------

		BronzeShield&	operator=(const BronzeShield& other) = delete;

		// Getters and setters ------------------------------------------------

		// TODO: Add getter to static var

		// Utils --------------------------------------------------------------

		// TODO: Funcs
};

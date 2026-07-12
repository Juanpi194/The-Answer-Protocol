#pragma once

#include "items/Shield.hpp"

class IronShield final: public Shield
{
	private:
		static unsigned int				available_id;	// Defined in IronShield.cpp
		static const std::string		PREFIX;	// Defined in IronShield.cpp
		static const std::string		NAME;	// Defined in IronShield.cpp
		static const std::string		DESCRIPTION;	// Defined in IronShield.cpp
		static constexpr unsigned int	MAX_USES = 5;
	public:
		// Constructors -------------------------------------------------------

		IronShield(void);
		IronShield(const IronShield& iron_shield);
		~IronShield(void) = default;

		// Operators ----------------------------------------------------------

		IronShield&	operator=(const IronShield& other) = delete;

		// Getters and setters ------------------------------------------------

		// TODO: Add getter to static var

		// Utils --------------------------------------------------------------

		// TODO: Funcs
};

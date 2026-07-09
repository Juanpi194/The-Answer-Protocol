#pragma once

#include "items/Shield.hpp"

class IronShield final: public Shield
{
	private:
		static const std::string		NAME;	// Defined in IronArmor.cpp
		static const std::string		DESCRIPTION;	// Defined in IronArmor.cpp
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

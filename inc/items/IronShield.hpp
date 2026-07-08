#pragma once

#include "items/Shield.hpp"

class IronShield final: public Shield
{
	private:
		static constexpr unsigned int	MAX_USES = 5;
	public:
		// Constructors -------------------------------------------------------

		IronShield(const std::string& id, const std::string& name, const std::string& description);
		IronShield(const IronShield& iron_shield) = delete;
		~IronShield(void) = default;

		// Operators ----------------------------------------------------------

		IronShield&	operator=(const IronShield& other) = delete;

		// Getters and setters ------------------------------------------------

		// TODO: Add getter to static var

		// Utils --------------------------------------------------------------

		// TODO: Funcs
};

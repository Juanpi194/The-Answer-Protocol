#pragma once

#include "enchantments/Enchantment.hpp"
#include "characters/Fighter.hpp"

class Freeze final: public Enchantment
{
	private:
		static const std::list<GearType>	ALLOWED_GEAR_TYPES;	// Defined in Freeze.cpp
	public:
		static const std::string			NAME;	// Defined in Freeze.cpp
		static const std::string			DESCRIPTION;	// Defined in Freeze.cpp
		static constexpr Status				APPLIED_STATUS = Status::FROZEN;

		// Constructors -------------------------------------------------------

		Freeze(void);
		Freeze(const Freeze& flame);
		~Freeze(void) = default;
		Freeze	*clone(void) const noexcept override;

		// Operators ----------------------------------------------------------
	
		Freeze&	operator=(const Freeze& other) = delete;

		// Getters and setters ------------------------------------------------

		// TODO: Add getters and setters for future attributes.

		// Utils --------------------------------------------------------------

		void	effect(Fighter& user, Fighter& affected) const noexcept override;
};

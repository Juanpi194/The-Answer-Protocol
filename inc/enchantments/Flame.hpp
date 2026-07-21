#pragma once

#include "enchantments/Enchantment.hpp"

class Flame final: public Enchantment
{
	private:
		static const std::list<GearType>	ALLOWED_GEAR_TYPES;	// Defined in Flame.cpp
	public:
		static const std::string			NAME;	// Defined in Flame.cpp
		static const std::string			DESCRIPTION;	// Defined in Flame.cpp

		// Constructors -------------------------------------------------------

		Flame(void);
		Flame(const Flame& flame);
		~Flame(void) = default;
		Flame	*clone(void) const noexcept override;

		// Operators ----------------------------------------------------------
	
		Flame&	operator=(const Flame& other) = delete;

		// Getters and setters ------------------------------------------------

		// TODO: Add getters and setters for future attributes.

		// Utils --------------------------------------------------------------

		void	effect(void) noexcept override;
};

#pragma once

#include "enchantments/Enchantment.hpp"
#include "characters/Fighter.hpp"

class Flame final: public Enchantment
{
	private:
		static const std::list<GearType>	ALLOWED_GEAR_TYPES;	// Defined in Flame.cpp
	public:
		static const std::string			NAME;	// Defined in Flame.cpp
		static const std::string			DESCRIPTION;	// Defined in Flame.cpp
		static constexpr Status				APPLIED_STATUS = Status::BURNT;

		// Constructors -------------------------------------------------------

		Flame(void);
		Flame(const Flame& flame);
		~Flame(void) = default;
		Flame	*clone(void) const noexcept override;

		// Operators ----------------------------------------------------------
	
		Flame&	operator=(const Flame& other) = delete;

		// Utils --------------------------------------------------------------

		void	effect(Fighter& user, Fighter& affected) const noexcept override;
};

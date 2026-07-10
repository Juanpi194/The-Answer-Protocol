#pragma once

#include "items/Weapon.hpp"

class IronSword final: public Weapon
{
	private:
		// TODO: Add attributes

		static const std::string		NAME;	// Defined in IronSword.cpp
		static const std::string		DESCRIPTION;	// Defined in IronSword.cpp
		static constexpr unsigned int	EXTRA_DAMAGE = 5;
		static constexpr GearType		GEAR_TYPE = GearType::Iron;

		void	special_use(void) noexcept override;
	public:
		// Operators ----------------------------------------------------------

		IronSword(void);
		IronSword(const IronSword& iron_sword);
		~IronSword(void) = default;

		// Operators ----------------------------------------------------------

		IronSword&	operator=(const IronSword& other) = delete;

		// Getters and setters ------------------------------------------------

		// TODO: Add getters and setters for future attributes.

		// Utils --------------------------------------------------------------
};

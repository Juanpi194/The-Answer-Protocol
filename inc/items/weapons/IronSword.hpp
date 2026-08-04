#pragma once

#include "items/Weapon.hpp"

class IronSword final: public Weapon
{
	private:
		static unsigned int				available_id;	// Defined in IronSword.cpp
		static const std::string		PREFIX;	// Defined in IronSword.cpp
		static constexpr GearType		GEAR_TYPE = GearType::IRON;

	public:
		static const std::string		NAME;	// Defined in IronSword.cpp
		static const std::string		DESCRIPTION;	// Defined in IronSword.cpp
		static constexpr unsigned int	EXTRA_DAMAGE = 7;

		// Constructors -------------------------------------------------------

		IronSword(void);
		IronSword(const IronSword& iron_sword);
		~IronSword(void) = default;
		IronSword	*clone(void) const noexcept override TAP_RETURNS_NONNULL;

		// Operators ----------------------------------------------------------

		IronSword&	operator=(const IronSword& other) = delete;

};

#pragma once

#include "items/Weapon.hpp"

class FlameSword final: public Weapon
{
	private:
		static unsigned int				available_id;	// Defined in FlameSword.cpp
		static const std::string		PREFIX;	// Defined in FlameSword.cpp
		static constexpr GearType		GEAR_TYPE = GearType::Fire;
	public:
		static const std::string		NAME;	// Defined in FlameSword.cpp
		static const std::string		DESCRIPTION;	// Defined in FlameSword.cpp
		static constexpr unsigned int	EXTRA_DAMAGE = 8;

		// Constructors -------------------------------------------------------

		FlameSword(void);
		FlameSword(const FlameSword& flame_sword);
		~FlameSword(void) = default;
		FlameSword	*clone(void) const noexcept override;

		// Operators ----------------------------------------------------------

		FlameSword&	operator=(const FlameSword& other) = delete;

		// Getters and setters ------------------------------------------------

		// TODO: Add getters and setters for future attributes.

		// Utils --------------------------------------------------
};

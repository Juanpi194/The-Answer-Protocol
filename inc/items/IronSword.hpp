#pragma once

#include "items/Weapon.hpp"

class IronSword final: public Weapon
{
	private:
		static unsigned int				available_id;	// Defined in IronSword.cpp
		static const std::string		PREFIX;	// Defined in IronSword.cpp
		static constexpr GearType		GEAR_TYPE = GearType::Iron;
		
		void	special_use(void) noexcept override;
	public:
		static const std::string		NAME;	// Defined in IronSword.cpp
		static const std::string		DESCRIPTION;	// Defined in IronSword.cpp
		static constexpr unsigned int	EXTRA_DAMAGE = 5;

		// Constructors -------------------------------------------------------

		IronSword(void);
		IronSword(const IronSword& iron_sword);
		~IronSword(void) = default;
		IronSword	*clone(void) const noexcept override;

		// Operators ----------------------------------------------------------

		IronSword&	operator=(const IronSword& other) = delete;

		// Getters and setters ------------------------------------------------

		// TODO: Add getters and setters for future attributes.

		// Utils --------------------------------------------------------------
};

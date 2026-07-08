#pragma once

#include "items/Weapon.hpp"

class IronSword: public Weapon
{
	private:
		// TODO: Add attributes

		static const std::string		NAME;	// Defined in IronSword.cpp
		static const std::string		DESCRIPTION;	// Defined in IronSword.cpp
		static constexpr unsigned int	EXTRA_DAMAGE = 5;

		void	special_use(void) noexcept override;
	public:
		// Operators ----------------------------------------------------------

		IronSword(const std::string& id);
		IronSword(const IronSword& sword) = delete;
		~IronSword(void);

		// Operators ----------------------------------------------------------

		IronSword&	operator=(const IronSword& other) = delete;

		// Getters and setters ------------------------------------------------

		// TODO: Add getters and setters for future attributes.

		// Utils --------------------------------------------------------------
};

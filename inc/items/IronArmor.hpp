#pragma once

#include "items/Armor.hpp"

class IronArmor: public Armor
{
	private:
		// TODO: Add attributes

		static const std::string		NAME;	// Defined in IronArmor.cpp
		static const std::string		DESCRIPTION;	// Defined in IronArmor.cpp
		static constexpr unsigned int	REDUCED_DAMAGE = 3;

		void	special_use(void) noexcept override;
	public:
		// Constructors -------------------------------------------------------

		IronArmor(const std::string& id);
		IronArmor(const IronArmor& iron_armor) = delete;
		~IronArmor(void);

		// Operators ----------------------------------------------------------

		IronArmor&	operator=(const IronArmor& other) = delete;

		// Getters and setters ------------------------------------------------

		// TODO: Add getters and setters for future attributes.

		// Utils --------------------------------------------------------------
};

#pragma once

#include "characters/Enemy.hpp"

class Wizard final: public Enemy
{
	private:
		static unsigned int			available_id;	// Defined in Wizard.cpp
		static const std::string	PREFIX;	// Definied in Wizard.cpp
	public:
		static const std::string	NAME;	// Defined in Wizard.cpp
		static const std::string	DESCRIPTION;	// Defined in Wizard.cpp
		static const t_stats		DEFAULT_STATS;	// Defined in Wizard.cpp
		static const unsigned int	DEFAULT_GOLD = 50;

		// Constructors -------------------------------------------------------

		Wizard(void);
		Wizard(const Wizard& wizard);
		~Wizard(void) = default;
		Wizard	*clone(void) const noexcept override TAP_RETURNS_NONNULL;

		// Operators ----------------------------------------------------------

		Wizard&	operator=(const Wizard& other) = delete;

		// Utils --------------------------------------------------------------

		FightChoice	choose_action(void) const noexcept override;
};

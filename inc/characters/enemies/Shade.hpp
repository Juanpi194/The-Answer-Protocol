#pragma once

#include "characters/Enemy.hpp"

class Shade final: public Enemy
{
	private:
		static unsigned int			available_id;	// Defined in Shade.cpp
		static const std::string	PREFIX;	// Definied in Shade.cpp
	public:
		static const std::string	NAME;	// Defined in Shade.cpp
		static const std::string	DESCRIPTION;	// Defined in Shade.cpp
		static const t_stats		DEFAULT_STATS;	// Defined in Shade.cpp
		static const unsigned int	DEFAULT_GOLD = 25;

		// Constructors -------------------------------------------------------

		Shade(void);
		Shade(const Shade& shade);
		~Shade(void) = default;
		Shade	*clone(void) const noexcept override TAP_RETURNS_NONNULL;

		// Operators ----------------------------------------------------------

		Shade&	operator=(const Shade& other) = delete;

		// Utils --------------------------------------------------------------

		FightChoice	choose_action(void) const noexcept override;
};

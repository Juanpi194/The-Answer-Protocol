#pragma once

#include "characters/Enemy.hpp"

class Dragon final: public Enemy
{
	private:
		static unsigned int			available_id;	// Defined in Dragon.cpp
		static const std::string	PREFIX;	// Definied in Dragon.cpp
	public:
		static const std::string	NAME;	// Defined in Dragon.cpp
		static const std::string	DESCRIPTION;	// Defined in Dragon.cpp
		static const t_stats		DEFAULT_STATS;	// Defined in Dragon.cpp
		static const unsigned int	DEFAULT_GOLD = 200;

		// Constructors -------------------------------------------------------

		Dragon(void);
		Dragon(const Dragon& dragon);
		~Dragon(void) = default;
		Dragon	*clone(void) const noexcept override TAP_RETURNS_NONNULL;

		// Operators ----------------------------------------------------------

		Dragon&	operator=(const Dragon& other) = delete;

		// Utils --------------------------------------------------------------

		FightChoice	choose_action(void) const noexcept override;
};

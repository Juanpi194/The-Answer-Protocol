#pragma once

#include "characters/enemies/Enemy.hpp"

class Goblin final: public Enemy
{
	private:
		static unsigned int			available_id;	// Defined in Goblin.cpp
		static const std::string	PREFIX;	// Definied in Goblin.cpp
	public:
		static const std::string	NAME;	// Defined in Goblin.cpp
		static const std::string	DESCRIPTION;	// Defined in Goblin.cpp
		static const t_stats		DEFAULT_STATS;	// Defined in Goblin.cpp
		static const unsigned int	DEFAULT_GOLD = 20;

		// Constructors -------------------------------------------------------

		Goblin(void);
		Goblin(const Goblin& goblin);
		~Goblin(void) = default;
		Goblin	*clone(void) const noexcept override TAP_RETURNS_NONNULL;

		// Operators ----------------------------------------------------------

		Goblin&	operator=(const Goblin& other) = delete;

		// Getters and setters ------------------------------------------------

		// TODO: Add specific attributes to the goblin and their getters and setters

		// Utils --------------------------------------------------------------

		FightChoice	choose_action(void) const noexcept override;
};

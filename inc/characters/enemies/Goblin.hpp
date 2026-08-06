#pragma once

#include "characters/Enemy.hpp"

class Goblin final: public Enemy
{
	private:
		static unsigned int			available_id;	// Defined in Goblin.cpp
		static const std::string	PREFIX;	// Definied in Goblin.cpp
	public:
		static const std::string	NAME;	// Defined in Goblin.cpp
		static const std::string	DESCRIPTION;	// Defined in Goblin.cpp
		static const t_stats		DEFAULT_STATS;	// Defined in Goblin.cpp
		static constexpr unsigned int	DEFAULT_GOLD = 20;
		static constexpr unsigned int	CONSUME_CHANCE = 30;

		// Constructors -------------------------------------------------------

		Goblin(void);
		Goblin(const Goblin& goblin);
		~Goblin(void) = default;
		Goblin	*clone(void) const noexcept override TAP_RETURNS_NONNULL;

		// Operators ----------------------------------------------------------

		Goblin&	operator=(const Goblin& other) = delete;

		// Utils --------------------------------------------------------------

		FightChoice	choose_action(void) noexcept override;
};

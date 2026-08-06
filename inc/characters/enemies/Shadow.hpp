#pragma once

#include "characters/Enemy.hpp"

class Shadow final: public Enemy
{
	private:
		static unsigned int			available_id;	// Defined in Shadow.cpp
		static const std::string	PREFIX;	// Definied in Shadow.cpp
	public:
		static const std::string	NAME;	// Defined in Shadow.cpp
		static const std::string	DESCRIPTION;	// Defined in Shadow.cpp
		static const t_stats		DEFAULT_STATS;	// Defined in Shadow.cpp
		static constexpr unsigned int	DEFAULT_GOLD = 70;
		static constexpr unsigned int	CONSUME_CHANCE = 15;

		// Constructors -------------------------------------------------------

		Shadow(void);
		Shadow(const Shadow& shadow);
		~Shadow(void) = default;
		Shadow	*clone(void) const noexcept override TAP_RETURNS_NONNULL;

		// Operators ----------------------------------------------------------

		Shadow&	operator=(const Shadow& other) = delete;

		// Utils --------------------------------------------------------------

		FightChoice	choose_action(void) noexcept override;
};

#pragma once

#include "characters/Fighter.hpp"

class Enemy;

class Battle
{
	private:
		Fighter&		blue;
		Fighter&		red;
		Enemy*			original_enemy;
		unsigned int	turns;
		bool			finished;

		void	execute_turn(void);
	public:
		// Constructors -------------------------------------------------------

		Battle(Fighter& blue, Fighter& red);
		Battle(const Battle& battle) = delete;
		~Battle(void) = default;

		// Operators ----------------------------------------------------------

		Battle&	operator=(const Battle& other) = delete;

		// Getters and setters ------------------------------------------------

		Fighter&		get_blue(void) noexcept;
		const Fighter&	get_blue(void) const noexcept;
		Fighter&		get_red(void) noexcept;
		const Fighter&	get_red(void) const noexcept;
		unsigned int	get_turns(void) const noexcept;
		bool			is_finished(void) const noexcept;

		// Utils --------------------------------------------------------------

};

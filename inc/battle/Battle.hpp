#pragma once

#include "characters/Fighter.hpp"

class Enemy;

/**
 * @brief	Class that simulates the battle between two fighters.
 * @note	FOR NOW, only a player as the blue fighter and an enemy
 * 			as the red fighter is supported.
 */
class Battle
{
	private:
		/**
		 * @brief	The player for now.
		 */
		Fighter&		blue;

		/**
		 * @brief	Copy of the enemy the player is facing. It will be freed
		 * 			by the destructor.
		 */
		Enemy			*enemy_copy;
		
		/**
		 * @brief	Pointer to the original enemy the player is facing.
		 * @note	It will not be freed by the destructor.
		 */
		Enemy			*original_enemy;

		/**
		 * @brief	An enemy for now.
		 */
		Fighter&		red;

		/**
		 * @brief	Starts as `nullptr`. Tells which Fighter won the fight.
		 */
		Fighter			*winner;

		unsigned int	turns;
		bool			finished;
	
		/**
		 * @brief	Sets a fighter as the winner of the battle, and also
		 * 			turns `finished` to `true`.
		 * @param	winner	The winner fighter.
		 */
		void	set_winner(Fighter& winner) noexcept;

		/**
		 * @brief	Performs the specified action by the actor.
		 * @param	actor		The fighter to perform the action.
		 * @param	opponent	`actor`'s enemy.
		 * @param	choice		The selected choice by `actor`.
		 */
		void	resolve_action(Fighter& actor, Fighter& opponent, FightChoice choice) noexcept;
	public:
		// Constructors -------------------------------------------------------

		Battle(Fighter& blue, Fighter& red) TAP_ERROR("Battle between players is not supported yet.");
		Battle(Player& player, Enemy *enemy) TAP_NONNULL;
		Battle(const Battle& battle) = delete;
		~Battle(void);

		// Operators ----------------------------------------------------------

		Battle&	operator=(const Battle& other) = delete;

		// Getters and setters ------------------------------------------------

		/**
		 * @brief	Returns the blue side, which is the player's.
		 * @returns	The player.
		 */
		const Fighter&	get_blue(void) const noexcept;

		/**
		 * @brief	Returns the red side, which is the enemy's.
		 * @returns	The enemy.
		 */
		const Fighter&	get_red(void) const noexcept;

		Fighter			*get_winner(void) const noexcept;
		unsigned int	get_turns(void) const noexcept;
		bool			is_finished(void) const noexcept;
		Enemy			*get_original_enemy(void) const noexcept;

		void	set_finished(bool finished) noexcept;

		// Utils --------------------------------------------------------------

		/**
		 * @brief	Executes a battle turn.
		 * @param	player_action	The action chosen by the
		 * 							player (Blue side).
		 */
		void		execute_turn(FightChoice player_choice) noexcept;

		std::string	to_json_format(void) const noexcept;
};

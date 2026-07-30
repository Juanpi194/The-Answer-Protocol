#include "battle/Battle.hpp"

#include <stdexcept>

#include "utils/utils.hpp"
#include "characters/enemies/Enemy.hpp"
#include "characters/Player.hpp"

void	Battle::set_winner(Fighter& winner) noexcept
{
	log("Fighter '" + winner.get_name() + "' won the fight.", LogLevel::INFO);
	this->winner = &winner;
	finished = true;
}

void	Battle::resolve_action(Fighter& actor, Fighter& opponent, FightChoice choice) noexcept
{
	if (choice.action == FightAction::FLEE)
	{
		if (actor.flee(opponent))
			finished = true;
	}
	else
	{
		actor.perform_action(opponent, choice);
		if (actor.get_stats().current_hp == 0)
			set_winner(opponent);
		else if (opponent.get_stats().current_hp == 0)
			set_winner(actor);
	}
}

// Constructors ---------------------------------------------------------------

Battle::Battle(Fighter& blue, Fighter& red):
	blue(blue),
	enemy_copy(nullptr),
	original_enemy(nullptr),
	red(red),
	winner(nullptr),
	turns(0),
	finished(false)
{
	// ? REVIEW: Check this logic.
	if (&blue == &red)
		throw std::invalid_argument("Cannot start a battle with the same two fighters.");
	log("Battle between '" + blue.get_name() + "' and '" + red.get_name() + "' starts!", LogLevel::INFO);
}


Battle::Battle(Player& player, Enemy *enemy):
	blue(player),
	enemy_copy(enemy->clone()),
	original_enemy(enemy),
	red(*enemy_copy),
	winner(nullptr),
	turns(0),
	finished(false)
{
	log("Battle between '" + blue.get_name() + "' and '" + enemy->get_name() + "' starts!", LogLevel::INFO);
}

Battle::~Battle(void)
{
	if (enemy_copy)
		delete (enemy_copy);
}

// Getters and setters --------------------------------------------------------

const Fighter&	Battle::get_blue(void) const noexcept
{
	return (blue);
}

const Fighter&	Battle::get_red(void) const noexcept
{
	return (red);
}

Fighter			*Battle::get_winner(void) const noexcept
{
	return (winner);
}

unsigned int	Battle::get_turns(void) const noexcept
{
	return (turns);
}

bool			Battle::is_finished(void) const noexcept
{
	return (finished);
}

void	Battle::set_finished(bool finished) noexcept
{
	this->finished = finished;
}

// Utils ----------------------------------------------------------------------

void	Battle::execute_turn(FightChoice player_choice) noexcept
{
	Fighter		*first;
	Fighter		*second;
	FightChoice	first_choice;
	FightChoice	second_choice;

	// Enemy choice
	FightChoice enemy_choice = enemy_copy->choose_action();

	// Setting turn order...
	if (blue.get_stats().current_speed >= red.get_stats().current_speed || player_choice.action == FightAction::DEFEND)
	{
		first = &blue;
		second = enemy_copy;
		first_choice = player_choice;
		second_choice = enemy_choice;
	}
	else
	{
		first = enemy_copy;
		second = &blue;
		first_choice = enemy_choice;
		second_choice = player_choice;
	}

	// The turn...
	try
	{
		// First fighter acting
		resolve_action(*first, *second, first_choice);

		// Finished?
		if (finished)
			return;

		// Second fighter acting
		resolve_action(*second, *first, second_choice);

		// End of turn variables.
		turns++;
		blue.set_last_action(player_choice.action);
		red.set_last_action(enemy_choice.action);
		blue.set_defending(false);
		red.set_defending(false);
	}
	catch (const std::invalid_argument& e)
	{
		// TODO: Backup for perform_action error.
	}
}

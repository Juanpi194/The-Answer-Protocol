#include "battle/Battle.hpp"

#include <stdexcept>

#include "utils/utils.hpp"

void	Battle::execute_turn(void)
{
	// TODO: Logic...
	blue.choose_action();
	red.choose_action();
}

// Constructors ---------------------------------------------------------------

Battle::Battle(Fighter& blue, Fighter& red):
	blue(blue),
	red(red),
	turns(0),
	finished(false)
{
	// ? REVIEW: Check this logic.
	if (&blue == &red)
		throw std::invalid_argument("Cannot start a battle with the same two fighters.");
}

// Getters and setters --------------------------------------------------------

Fighter&		Battle::get_blue(void) noexcept
{
	return (blue);
}

const Fighter&	Battle::get_blue(void) const noexcept
{
	return (blue);
}

Fighter&		Battle::get_red(void) noexcept
{
	return (red);
}

const Fighter&	Battle::get_red(void) const noexcept
{
	return (red);
}

unsigned int	Battle::get_turns(void) const noexcept
{
	return (turns);
}

bool			Battle::is_finished(void) const noexcept
{
	return (finished);
}

// Utils ----------------------------------------------------------------------

void	Battle::start_battle(void) noexcept
{
	// TODO: Battle logic
	log("Battle between '" + blue.get_name() + "' and '" + red.get_name() + "' starts!", LogLevel::INFO);
	while (true)
	{
		break ;
	}
}

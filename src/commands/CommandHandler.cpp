#include "commands/CommandHandler.hpp"

#include "characters/Player.hpp"

void	CommandHandler::handle(Player& player, World& world, std::string text)
{
	// TODO: Parse...
	if (text == "MOVE NORTH")
		player.move(Direction::NORTH);
	else if (text == "MOVE EAST")
		player.move(Direction::EAST);
	else if (text == "MOVE SOUTH")
		player.move(Direction::SOUTH);
	else if (text == "MOVE WEST")
		player.move(Direction::WEST);

	// TODO: Game logic...
}

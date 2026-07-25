#include "commands/CommandHandler.hpp"

#include "utils/utils.hpp"
#include "characters/Player.hpp"
#include "characters/Enemy.hpp"
#include "world/Room.hpp"

void	CommandHandler::handle(Player& player, World& world, std::string text)
{
	// TODO: Parse...
	// TODO: Add specific errors in case something wrong happens.
	// ? REVIEW: Recheck messages sent to the user.
	if (text == "MOVE NORTH")
		player.move(Direction::NORTH);
	else if (text == "MOVE EAST")
		player.move(Direction::EAST);
	else if (text == "MOVE SOUTH")
		player.move(Direction::SOUTH);
	else if (text == "MOVE WEST")
		player.move(Direction::WEST);
	else if (text == "ATTACK")
	{
		// ? REVIEW: All this logic.
		NPC		*npc;
		Enemy	*enemy;

		npc = player.get_current_room()->get_NPC();
		if (!npc)
		{
			player.send_to_outbox("There is no enemy in this room.");
			return ;
		}
		enemy = dynamic_cast<Enemy*>(npc);
		if (!enemy)
		{
			player.send_to_outbox("There is no enemy in this room.");
			return ;
		}
		log("'" + player.get_name() + "' attacks '" + enemy->get_name() + "'.", LogLevel::INFO);
		player.attack(*enemy);
	}
	// TODO: Game logic...
}

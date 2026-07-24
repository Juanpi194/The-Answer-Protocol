#pragma once
#include <string>

class Player;
class World;

class CommandHandler
{
	public:
		/**
		 * @brief	Handles the message sent by the player.
		 */
		static void	handle(Player& player, World& world, std::string text);
};

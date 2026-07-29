#pragma once
#include <string>

#include "commands/command.hpp"

class PlayerConnection;
class World;

class CommandHandler
{
	public:
		/**
		 * @brief	Handles the command sent by the player.
		 */
		static void	handle(const Command& cmd, PlayerConnection& connection, World& world);
};

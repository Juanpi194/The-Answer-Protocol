// TODO: Change this file's name to main.cpp
#include <string>
#include <sys/socket.h>
#include <sys/time.h>
#include <cerrno>
#include <iostream>
#include <functional>
#include "utils/utils.hpp"
#include "commands/command.hpp"
#include "commands/CommandHandler.hpp"
#include "commands/commandparser.hpp"
#include "server/PlayerConnection.hpp"
#include "server/Server.hpp"
#include "server/ServerOwner.hpp"
#include "world/World.hpp"
const std::string	DEFAULT_CLIENT_NAME = "Alberto";
const std::string	DEFAULT_WORLD_NAME = "The Amazing World Of Gumball";
const std::string	DEFAULT_WORLD_JSON = "default.json";
static bool	read_command(std::string& out)
{
	std::getline(std::cin, out);
	if (std::cin.eof())
	{
		std::cin.clear();
		return (false);
	}
	if (std::cin.fail())
		throw std::runtime_error("Error reading input.");
	return (true);
}
static void	debug_mode(void)
{
	World					world(DEFAULT_WORLD_NAME, DEFAULT_WORLD_JSON);
	PlayerConnection		client(DEFAULT_CLIENT_NAME, -1, nullptr);
	std::string				msg;
	Command					cmd;
	world.get_spawn_room()->add_player(&client.get_player());
	client.connect();
	while (client.is_connected())
	{
		if (!read_command(msg))
		{
			client.disconnect();
			break;
		}
		try
		{
			cmd = CommandParser::parse(msg);
		}
		catch (const CommandParseError& e)
		{
			std::cout << e.what() << std::endl;
			continue;
		}
		CommandHandler::handle(cmd, client, world);
		for (const std::string& outbox_msg: client.get_player().drain_outbox())
			std::cout << outbox_msg << std::endl;
		if (client.is_quitting())
			client.disconnect();
	}
}
static void	normal_mode(void)
{
	Server		server;
	World		world(DEFAULT_WORLD_NAME, DEFAULT_WORLD_JSON);
	ServerOwner	owner("Yanpi", &server);
	server.set_world(&world);
	// TODO: Create owner here, ask for name, then instantiate, ...
	std::thread	owner_thread(&ServerOwner::owner_thread, &owner);
	server.game_loop();
	owner_thread.join();
}
int	main(void)
{
	// new int(5);
	try
	{
		if (current_level == LogLevel::DEBUG)
			debug_mode();
		else
			normal_mode();
	}
	catch (const std::invalid_argument& e)
	{
		// TODO: Specific behavior
		std::cerr << e.what() << std::endl;
		return (1);
	}
	catch (const std::runtime_error& e)
	{
		// TODO: Specific behavior
		std::cerr << e.what() << std::endl;
		return (1);
	}
	catch (const std::bad_alloc& e)
	{
		// TODO: Specific behavior
		std::cerr << e.what() << std::endl;
		return (1);
	}
	catch (const std::exception& e)
	{
		// TODO: Specific behavior
		std::cerr << e.what() << std::endl;
		return (1);
	}
	return (0);
}

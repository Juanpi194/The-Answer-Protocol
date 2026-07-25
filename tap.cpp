// TODO: Change this file's name to main.cpp
#include <string>
#include <sys/socket.h>
#include <sys/time.h>
#include <cerrno>
#include <iostream>
#include <functional>

#include "utils/utils.hpp"
#include "commands/CommandHandler.hpp"
#include "server/PlayerConnection.hpp"
#include "server/Server.hpp"
#include "server/ServerOwner.hpp"
#include "world/World.hpp"

const std::string	DEFAULT_CLIENT_NAME = "Alberto";
const std::string	DEFAULT_WORLD_NAME = "The Amazing World Of Gumball";
const std::string	DEFAULT_WORLD_JSON = "default.json";

static void	debug_mode(void)
{
	World					world(DEFAULT_WORLD_NAME, DEFAULT_WORLD_JSON);
	PlayerConnection		client(DEFAULT_CLIENT_NAME, -1, nullptr);
	std::string				msg;
	std::list<std::string>	outbox_msgs;

	world.get_spawn_room()->add_player(&client.get_player());
	client.connect();
	while (client.is_connected())
	{
		std::getline(std::cin, msg);
		if (std::cin.eof())
		{
			std::cin.clear();
			client.disconnect();
			break ;
		}
		if (std::cin.fail())
			throw std::runtime_error("Error reading input.");
		CommandHandler::handle(client.get_player(), world, msg);
		outbox_msgs = client.get_player().drain_outbox();
		for (const std::string& outbox_msg: outbox_msgs)
			std::cout << outbox_msg << std::endl;
	}
}

static void	normal_mode(void)
{
	Server		server;
	World		world(DEFAULT_WORLD_NAME, DEFAULT_WORLD_JSON);
	ServerOwner	owner("Yanpi", &server);

	server.set_world(&world);
	// TODO: Create owner here, ask for name, then instantiate, ...
	std::thread	owner_thread(&ServerOwner::owner_thread, &owner, std::ref(server));
	server.game_loop();
	owner_thread.join();
}

int	main(void)
{
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

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
const std::string	DEFAULT_WORLD_JSON = "map/default.json"; // MODIFIED: antes "default.json" en la raíz
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
	// MODIFIED: connect() ya no existe en PlayerConnection -- el
	// constructor deja `connected` en true directamente.
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
// MODIFIED: antes se decidía en tiempo de compilación (current_level, via
// #ifdef DEBUG_BUILD), lo que obligaba a compilar dos veces todo el
// proyecto (una carpeta de objetos por modo) para poder tener los dos
// binarios. Ahora es un flag de línea de comandos, así que un único set de
// objetos sirve para todo -- "tap" sin nada es el server, "tap --debug"
// entra en debug_mode.
int	main(int argc, char **argv)
{
	bool	debug_requested = false;

	for (int i = 1; i < argc; ++i)
	{
		if (std::string(argv[i]) == "--debug" || std::string(argv[i]) == "-d")
			debug_requested = true;
	}
	try
	{
		if (debug_requested)
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

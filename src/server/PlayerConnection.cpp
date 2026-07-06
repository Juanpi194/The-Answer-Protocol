#include "server/PlayerConnection.hpp"

#include <iostream>

#include "server/Server.hpp"
#include "utils/utils.hpp"

// Constructors ---------------------------------------------------------------

PlayerConnection::PlayerConnection(const std::string& name, int client_fd, Server *server):
	player(name),
	client_fd(client_fd),
	connected(true),
	server(server)
{
	if (!server)
		throw std::invalid_argument("Player connection must be addressed to a server when being created.");
	// TODO: Check if player name or fd is already in the server
}

PlayerConnection::~PlayerConnection(void)
{
	// TODO: Close fd, ...
}

Player&			PlayerConnection::get_player(void) noexcept
{
	return (player);
}

const Player&	PlayerConnection::get_player(void) const noexcept
{
	return (player);
}

int				PlayerConnection::get_client_fd(void) const noexcept
{
	return (client_fd);
}

bool			PlayerConnection::is_connected(void) const noexcept
{
	return (connected);
}

Server			*PlayerConnection::get_server(void) const noexcept
{
	if (!server)
		log("Player " + player.get_name() + " is not connected to any server.", LogLevel::WARNING);
	return (server);
}

void	PlayerConnection::set_client_fd(int client_fd)
{
	// TODO: Check if is already in use, ...
	this->client_fd = client_fd;
}

void	PlayerConnection::set_connected(bool connected) noexcept
{
	this->connected = connected;
}

void	PlayerConnection::set_server(Server *server)
{
	// TODO: Check if server can accept new members, ...
	this->server = server;
}

void	PlayerConnection::connect(void)
{
	// TODO: Connect to the server, ...
}

void	PlayerConnection::disconnect(void)
{
	// TODO: Disconnect from the server ...
}

#include "server/PlayerConnection.hpp"

#include <stdexcept>
#include <iostream>
#include <unistd.h>

#include "server/Server.hpp"
#include "characters/Player.hpp"
#include "utils/utils.hpp"

// Constructors ---------------------------------------------------------------

PlayerConnection::PlayerConnection(const std::string& name, int client_fd, Server *server):
	player(Player(name)),
	client_fd(client_fd),
	connected(true),
	server(server),
	quitting(false),
	group(nullptr)
{
	// if (!server)
	// 	throw std::invalid_argument("Player connection must be addressed to a server when being created.");
	// TODO: Check if player name or fd is already in the server
}

PlayerConnection::~PlayerConnection(void)
{
	// TODO: Close fd, ...
	if (client_fd >= 0)
		close(client_fd);
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

bool			PlayerConnection::is_quitting(void) const noexcept
{
	return (quitting);
}

Group			*PlayerConnection::get_group(void) const noexcept
{
	return (group);
}

void	PlayerConnection::set_client_fd(int client_fd) noexcept
{
	if (server && server->is_fd_available(client_fd))
		this->client_fd = client_fd;
	else
	{
		log("Couldn't set fd " + std::to_string(client_fd) + " to '" + player.get_name() + "'.", LogLevel::WARNING);
		this->client_fd = -1;
	}
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

void	PlayerConnection::set_quitting(bool quitting) noexcept
{
	this->quitting = quitting;
}

void	PlayerConnection::set_group(Group *group) noexcept
{
	this->group = group;
}

void	PlayerConnection::reconnect(int fd)
{
	set_client_fd(fd);
	connected = true;
	quitting = false;
}

void	PlayerConnection::disconnect(void)
{
	quitting = false;
	connected = false;
	if (client_fd >= 0)
		close(client_fd);
	client_fd = -1;
}

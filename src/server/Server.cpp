#include "server/Server.hpp"

#include <netinet/in.h>

#include "utils/utils.hpp"

const std::string	ServerError::DEFAULT_MSG = "Server initialization failed.";

ServerError::ServerError(const std::string& msg)
{
	std::string	temp;

	temp = msg;
	trim_str(temp);
	if (temp.empty())
		this->msg = DEFAULT_MSG;
	else
		this->msg = temp;
}

const char	*ServerError::what(void) const noexcept
{
	return (msg.c_str());
}

int	Server::init(void)
{
	int			sock;
	sockaddr_in	address;

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == -1)
		return (log("Error in the socket creation.", LogLevel::ERROR), -1);
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(DEFAULT_PORT);
	if (bind(sock, (sockaddr *)&address, sizeof(address)) == -1)
		return (log("Error in the socket binding (Most likely because of port).", LogLevel::ERROR), -1);
	if (listen(sock, MAX_USERS) == -1)
		return (log("Error preparing socket connection.", LogLevel::ERROR), -1);
	return (sock);
}

// Constructors ---------------------------------------------------------------

Server::Server(void):
	sock(-1),
	owner(nullptr),
	world(nullptr),
	on(false)
{
	// TODO: Give json_path so a specific World can be created
}

Server::~Server(void)
{
	// TODO: Inform and disconnect every player, ...
}

// Getters abd setters --------------------------------------------------------

ServerOwner							*Server::get_server_owner(void) const noexcept
{
	return (owner);
}

World								*Server::get_world(void) const noexcept
{
	return (world);
}

std::list<PlayerConnection>&		Server::get_players(void) noexcept
{
	return (players);
}

const std::list<PlayerConnection>&	Server::get_players(void) const noexcept
{
	return (players);
}

bool								Server::is_on(void) const noexcept
{
	return (on);
}

void	Server::set_owner(ServerOwner *owner) noexcept
{
	if (!owner)
		log("Server owner established as nullptr.", LogLevel::INFO);
	this->owner = owner;
}

void	Server::set_world(World *world) noexcept
{
	if (!world)
		log("World established as nullptr.", LogLevel::INFO);
	this->world = world;
}

// Utils ----------------------------------------------------------------------

void	Server::start(void)
{
	if (on)
	{
		log("Tried to start server, but it is already on.", LogLevel::WARNING);
		return ;
	}
	sock = init();
	if (sock == -1)
		throw ServerError("Server initialization error.");
	on = true;
}

void	Server::stop(void)
{
	if (!on)
	{
		log("Tried to stop server, but it is already off.", LogLevel::WARNING);
		return ;
	}
	// TODO: Close fd, set it to -1, set 'on' to false, ...
}

void	Server::send_msg_to(int dst, const std::string& msg)
{
	// TODO: Send a message to a specific destiny fd.
}

void	Server::broadcast(const std::string& msg)
{
	// TODO: Send a message to all players.
}

void	Server::connect_player(PlayerConnection& player)
{
	// TODO: Connect a specific player
}

void	Server::disconnect_player(PlayerConnection& player)
{
	// TODO: Disconnect a specific player
}

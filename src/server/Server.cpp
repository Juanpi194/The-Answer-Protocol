#include "server/Server.hpp"

#include <netinet/in.h>
#include <cstring>
#include <thread>
#include <unistd.h>

#include "utils/utils.hpp"
#include "commands/CommandHandler.hpp"

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

void	Server::client_thread(int fd)
{
	PlayerConnection		*client;
	struct timeval			timeout;
	char					msg[MAX_MSG_LENGTH];
	ssize_t					bytes;
	std::list<std::string>	outbox_msgs;

	// TODO: Reconnect player or make a new connection by reading its name.
	// This is just a small example of a new player connected.
	{
		std::lock_guard<std::mutex>	lock(players_mtx);
		players.emplace_back("Juanpi", fd, this);
		client = &players.back();
	}

	// After the PlayerConnection is initiated, all this is correct.
	const int				client_fd = client->get_client_fd();
	timeout.tv_sec = 0;
	timeout.tv_usec = 100000;
	setsockopt(client_fd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
	while (client->is_connected())
	{
		memset(msg, 0, MAX_MSG_LENGTH);
		bytes = recv(client_fd, msg, MAX_MSG_LENGTH - 1, 0);
		if (bytes == 0)
		{
			client->disconnect();
			break ;
		}
		else if (bytes > 0)
			client->get_server()->push_command({client, std::string(msg, bytes)});
		else
		{
			if (errno != EAGAIN && errno != EWOULDBLOCK)
			{
				client->disconnect();
				break ;
			}
		}
		outbox_msgs = client->get_player().drain_outbox();
		for (const std::string& outbox_msg: outbox_msgs)
		{
			bytes = send(client_fd, outbox_msg.c_str(), outbox_msg.size(), MSG_NOSIGNAL);
			if (bytes == -1)
			{
				client->disconnect();
				break;
			}
		}
	}
}

void	Server::accept_loop(void)
{
	int	fd;

	while (true)
	{
		fd = accept(sock, nullptr, nullptr);
		if (fd < 0)
		{
			if (!on)
				break ;	// Server was closed.
			continue ;	// Error while accepting.
		}
		std::thread	thread(&Server::client_thread, this, fd);
		log("New thread with fd " + std::to_string(fd) + " launched.", LogLevel::DEBUG);
		thread.detach();
	}
}

// Constructors ---------------------------------------------------------------

Server::Server(void):
	sock(-1),
	owner(nullptr),
	world(nullptr),
	on(false),
	running(true)
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

void	Server::set_running(bool running) noexcept
{
	this->running = running;
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
	accept_thread = std::thread(&Server::accept_loop, this);
}

void	Server::stop(void)
{
	if (!on)
	{
		log("Tried to stop server, but it is already off.", LogLevel::WARNING);
		return ;
	}
	// TODO: Close fd, set it to -1, set 'on' to false, ...
	on = false;
	close(sock);
	sock = -1;
	accept_thread.join();
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

void	Server::push_command(const t_command& cmd)
{
	std::lock_guard<std::mutex>	lock(cmd_mtx);

	cmd_queue.push_back(cmd);
}

void	Server::game_loop(void)
{
	t_command	cmd;
	bool		got_one;

	while (running)
	{
		got_one = false;
		{
			std::lock_guard<std::mutex>	lock(cmd_mtx);
			if (!cmd_queue.empty())
			{
				cmd = cmd_queue.front();
				cmd_queue.pop_front();
				got_one = true;
			}
		}
		if (got_one && cmd.sender->is_connected())
		{
			assert(world != nullptr && "World cannot be nullptr.");
			CommandHandler::handle(cmd.sender->get_player(), *world, cmd.text);
		}
		else
			std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}
}

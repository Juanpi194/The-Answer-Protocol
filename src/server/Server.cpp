#include "server/Server.hpp"

#include <netinet/in.h>
#include <cstring>
#include <thread>
#include <unistd.h>

#include "utils/utils.hpp"
#include "commands/CommandHandler.hpp"
#include "commands/commandparser.hpp"

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
	int				sock;
	sockaddr_in		address;
	struct timeval	timeout;
	const int		opt = 1;

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == -1)
		return (log("Error in the socket creation.", LogLevel::ERROR), -1);

	// For timeouts ('accept' function)
	timeout.tv_sec = 0;
	timeout.tv_usec = 100000;
	setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));	// To reuse the port.

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(DEFAULT_PORT);
	if (bind(sock, (sockaddr *)&address, sizeof(address)) == -1)
		return (log("Error in the socket binding (Most likely because of port).", LogLevel::ERROR), -1);
	if (listen(sock, MAX_CLIENTS) == -1)
		return (log("Error preparing socket connection.", LogLevel::ERROR), -1);
	return (sock);
}

PlayerConnection	*Server::setup_client(int fd)
{
	char				answer[MAX_MSG_LENGTH];
	std::string			str_answer;
	PlayerConnection	*client;
	Command				cmd;
	ssize_t				bytes;
	bool				reconnecting;

	if (fd < 0)
		return (nullptr);
	if (send(fd, "Hello\n", strlen("Hello\n"), 0) == -1)
		return (nullptr);
	client = nullptr;
	reconnecting = false;
	while (!client && on)
	{
		memset(answer, 0, MAX_MSG_LENGTH);
		bytes = recv(fd, answer, MAX_MSG_LENGTH - 1, 0);
		if (bytes == -1)
		{
			if (errno == EAGAIN || errno == EWOULDBLOCK)
				continue;
			return (nullptr);
		}
		if (bytes == 0)
			return (nullptr);
		str_answer = std::string(answer, bytes);
		try
		{
			cmd = CommandParser::parse(str_answer);
		}
		catch (const CommandParseError& e)
		{
			if (send(fd, e.what(), strlen(e.what()), 0) == -1)
				return (nullptr);
			continue;
		}
		if (cmd.type != CommandType::CONNECT)
		{
			if (send(fd, "CONNECT before any other action\n", strlen("CONNECT before any other action\n"), 0) == -1)
				return (nullptr);
		}
		else if (cmd.args.empty())
		{
			if (send(fd, "CONNECT needs the name of the user to connect as an argument.\n", strlen("CONNECT needs the name of the user to connect as an argument.\n"), 0) == -1)
				return (nullptr);
		}
		else if (cmd.args.size() > 1)
		{
			if (send(fd, "CONNECT only needs the name of the user to connect as an argument.\n", strlen("CONNECT only needs the name of the user to connect as an argument.\n"), 0) == -1)
				return (nullptr);
		}
		else
		{
			// Connection
			{
				std::lock_guard<std::mutex>	lock(clients_mtx);
				client = search_client_by_name(cmd.args[0]);
				if (!client)
				{
					clients.emplace_back(cmd.args[0], fd, this);
					client = &clients.back();
				}
				else if (client->is_connected())
				{
					if (send(fd, "Name already in use.\n", strlen("Name already in use.\n"), 0) == -1)
						return (nullptr);
					client = nullptr;
				}
				else
					reconnecting = true;
			}
			if (reconnecting)
			{
				client->set_client_fd(fd);
				client->set_connected(true);
				client->set_quitting(false);
			}
		}
	}
	if (!client)
		return (nullptr);
	if (send(fd, "Connected\n", strlen("Connected\n"), 0) == -1)
		return (nullptr);
	connect_client(*client);
	return (client);
}

void	Server::client_thread(int fd)
{
	PlayerConnection		*client;
	struct timeval			timeout;
	char					msg[MAX_MSG_LENGTH];
	ssize_t					bytes;
	std::list<std::string>	outbox_msgs;

	client = setup_client(fd);
	if (!client)
		return ;

	if (!client->get_player().get_current_room())
		world->get_spawn_room()->add_player(&client->get_player());

	// After the PlayerConnection is initiated, all this is correct.
	const int				client_fd = client->get_client_fd();

	// For timeouts ('recv' function)
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
			break;
		}
		else if (bytes > 0)
			client->get_server()->push_command({client, std::string(msg, bytes)});
		else
		{
			if (errno != EAGAIN && errno != EWOULDBLOCK)
			{
				client->disconnect();
				break;
			}
		}
		outbox_msgs = client->get_player().drain_outbox();
		for (const std::string& outbox_msg: outbox_msgs)
		{
			bytes = send(client_fd, (outbox_msg + '\n').c_str(), (outbox_msg + '\n').size(), MSG_NOSIGNAL);
			if (bytes == -1)
			{
				client->disconnect();
				break;
			}
		}
		if (client->is_quitting())
		{
			client->disconnect();
			
		}
	}
	if (on)
		disconnect_client(*client);
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
				break;	// Server was closed.
			continue;	// Error while accepting.
		}
		log("Creating thread with fd " + std::to_string(fd) + "...", LogLevel::DEBUG);
		thread_list.emplace_back(&Server::client_thread, this, fd);
		log("New thread with fd " + std::to_string(fd) + " launched.", LogLevel::DEBUG);
	}
}

PlayerConnection	*Server::search_client_by_name(const std::string& name) noexcept
{
	for (PlayerConnection& client: clients)
		if (client.get_player().get_name() == name)
			return (&client);
	return (nullptr);
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
	if (on)
		stop();
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

const std::list<PlayerConnection>&	Server::get_clients(void) const noexcept
{
	return (clients);
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

	// Shutting down the server...
	on = false;
	close(sock);
	sock = -1;
	accept_thread.join();

	// Disconnecting players...
	{
		std::lock_guard<std::mutex>	lock(clients_mtx);
		for (PlayerConnection& client: clients)
			client.disconnect();
	}

	// Joining threads...
	for (std::thread& thread: thread_list)
		if (thread.joinable())
			thread.join();
}

void	Server::send_msg_to(int dst, const std::string& msg)
{
	PlayerConnection	*target;

	if (dst < 0)
		return;
	target = find_client_by_fd(dst);
	if (target)
		target->get_player().send_to_outbox(msg);	
}

void	Server::broadcast(const std::string& msg, int fd_excluded)
{
	std::lock_guard<std::mutex>	lock(clients_mtx);

	for (PlayerConnection& client: clients)
		if (client.get_client_fd() != fd_excluded)
			client.get_player().send_to_outbox(msg);
}

void	Server::connect_client(PlayerConnection& player)
{
	broadcast("Player '" + player.get_player().get_name() + "' connected.", player.get_client_fd());
}

void	Server::disconnect_client(PlayerConnection& player)
{
	broadcast("Player '" + player.get_player().get_name() + "' disconnected.", player.get_client_fd());
}

void	Server::push_command(const t_command& cmd)
{
	std::lock_guard<std::mutex>	lock(cmd_mtx);

	cmd_queue.push_back(cmd);
}

void	Server::game_loop(void)
{
	Command		cmd;
	t_command	cmd_info;
	bool		got_one;

	while (running)
	{
		got_one = false;
		{
			std::lock_guard<std::mutex>	lock(cmd_mtx);
			if (!cmd_queue.empty())
			{
				cmd_info = cmd_queue.front();
				cmd_queue.pop_front();
				got_one = true;
			}
		}
		if (got_one && cmd_info.sender->is_connected())
		{
			assert(world != nullptr && "World cannot be nullptr.");
			try
			{
				cmd = CommandParser::parse(cmd_info.text);
				CommandHandler::handle(cmd, *cmd_info.sender, *this->world);
			}
			catch (const CommandParseError& e)
			{
				(*cmd_info.sender).get_player().send_to_outbox(e.what());
				continue;
			}
		}
		else
			std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}
}

PlayerConnection	*Server::find_client_by_fd(int fd) noexcept
{
	std::lock_guard<std::mutex>	lock(clients_mtx);

	for (PlayerConnection& client: clients)
		if (client.get_client_fd() == fd)
			return (&client);
	return (nullptr);
}

bool				Server::is_fd_available(int fd) noexcept
{
	std::lock_guard<std::mutex>	lock(clients_mtx);

	if (fd < 0)
		return (false);
	for (PlayerConnection& client: clients)
		if (client.get_client_fd() == fd)
			return (false);
	return (true);
}

std::string			Server::list_clients(void) noexcept
{
	const std::string			bars = "=====";
	std::string					result;
	std::lock_guard<std::mutex>	lock(clients_mtx);

	result += bars;
	result += "\n";
	if (clients.size() == 0)
		result += "None\n";
	for (PlayerConnection& client: clients)
	{
		result += client.get_player().get_name() + ": ";
		if (client.is_connected())
			result += "Connected (fd: " + std::to_string(client.get_client_fd()) + ")\n";
		else
			result += "Disconnected\n";
	}
	result += bars;
	return (result);
}

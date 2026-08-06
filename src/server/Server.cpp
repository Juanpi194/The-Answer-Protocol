#include "server/Server.hpp"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <cstring>
#include <thread>
#include <unistd.h>

#include "utils/utils.hpp"
#include "commands/CommandHandler.hpp"
#include "commands/commandparser.hpp"
#include "group/Group.hpp"
#include "protocol/events.hpp"
#include "protocol/responses.hpp"

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

	sock = socket(DOMAIN, TYPE, 0);
	if (sock == -1)
		return (log("Error in the socket creation.", LogLevel::ERROR), -1);

	// For timeouts ('accept' function)
	timeout.tv_sec = 0;
	timeout.tv_usec = 100000;
	setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));	// To reuse the port.

	address.sin_family = DOMAIN;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(DEFAULT_PORT);
	if (bind(sock, (sockaddr *)&address, sizeof(address)) == -1)
		return (log("Error in the socket binding (Most likely because of port).", LogLevel::ERROR), -1);
	if (listen(sock, MAX_CLIENTS) == -1)
		return (log("Error preparing socket connection.", LogLevel::ERROR), -1);
	return (sock);
}

PlayerConnection	*Server::setup_client(const int fd)
{
	const std::string	welcome_msg = ok("Hello proto=" + std::to_string(fd));
	char				answer[MAX_MSG_LENGTH];
	std::string			str_answer;
	PlayerConnection	*client;
	Command				cmd;
	ssize_t				bytes;
	bool				reconnecting;
	std::string			msg_for_client;
	sockaddr_in			addr;
	socklen_t			len = sizeof(addr);
	char				ip[INET_ADDRSTRLEN];

	if (fd < 0)
		return (nullptr);

	// Getting ip
	getpeername(fd, (sockaddr *)&addr, &len);
	inet_ntop(AF_INET, &addr.sin_addr, ip, sizeof(ip));

	if (send(fd, welcome_msg.c_str(), welcome_msg.size(), 0) == -1)
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
		if (cmd.type == CommandType::QUIT)
		{
			msg_for_client = ok("bye");
			send(fd, msg_for_client.c_str(), msg_for_client.size(), 0);
			return (nullptr);
		}
		if (cmd.type != CommandType::CONNECT)
		{
			msg_for_client = err(ErrorCode::NOT_CONNECTED);
			if (send(fd, msg_for_client.c_str(), msg_for_client.size(), 0) == -1)
				return (nullptr);
		}
		else if (cmd.args.empty() || cmd.args.size() > 1)
		{
			msg_for_client = err(ErrorCode::WRONG_ARGUMENTS);
			if (send(fd, msg_for_client.c_str(), msg_for_client.size(), 0) == -1)
				return (nullptr);
		}
		else
		{
			// Connection
			{
				// Checking if client is banned.
				if (is_client_banned(cmd.args[0]))
				{
					msg_for_client = err(ErrorCode::BANNED_PLAYER);
					if (send(fd, msg_for_client.c_str(), msg_for_client.size(), 0) == -1)
						return (log(err(ErrorCode::SEND_FAILED), LogLevel::ERROR), nullptr);
					continue;
				}

				// Finding client.
				std::lock_guard<std::mutex>	lock(clients_mtx);
				client = search_client_by_name_nolock(cmd.args[0]);
				if (!client)
				{
					clients.emplace_back(cmd.args[0], fd, this);
					client = &clients.back();
				}
				else if (client->is_connected())
				{
					msg_for_client = err(ErrorCode::NAME_IN_USE);
					if (send(fd, msg_for_client.c_str(), msg_for_client.size(), 0) == -1)
						return (nullptr);
					client = nullptr;
				}
				else
					reconnecting = true;
			}
			if (reconnecting)
				client->reconnect(fd);
		}
	}
	if (!client)
		return (nullptr);
	client->set_ip(ip);
	log("Client connected: '" + client->get_player().get_name() + "' from " + ip, LogLevel::INFO);
	msg_for_client = ok("connected");
	if (send(fd, msg_for_client.c_str(), msg_for_client.size(), 0) == -1)
		return (nullptr);
	return (client);
}

void				Server::client_thread(int fd)
{
	PlayerConnection		*client;
	struct timeval			timeout;
	char					msg[MAX_MSG_LENGTH];
	ssize_t					bytes;
	std::list<std::string>	outbox_msgs;

	client = setup_client(fd);
	if (!client)
	{
		close(fd);
		return;
	}
	push_join(client);

	// After the PlayerConnection is initiated, all this is correct.
	const int	client_fd = client->get_client_fd();

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
			push_command({client, std::string(msg, bytes)});
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
			log("Response to '" + client->get_player().get_name() + "': " + outbox_msg, LogLevel::DEBUG);
			bytes = send(client_fd, (outbox_msg).c_str(), (outbox_msg).size(), MSG_NOSIGNAL);
			if (bytes == -1)
			{
				log(err(ErrorCode::SEND_FAILED), LogLevel::ERROR);
				client->disconnect();
				break;
			}
		}
		if (client->is_quitting())
			client->disconnect();
	}
	log("Client disconnected: '" + client->get_player().get_name() + "' from " + client->get_ip(), LogLevel::INFO);
	if (on)
		push_leave(client);
	client->disconnect();
}

void				Server::accept_loop(void)
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
		if (is_connection_flooding())
			log("Possible rapid connections (connection flooding)", LogLevel::WARNING);
		log("Creating thread with fd " + std::to_string(fd) + "...", LogLevel::DEBUG);
		thread_list.emplace_back(&Server::client_thread, this, fd);
		log("New thread with fd " + std::to_string(fd) + " launched.", LogLevel::DEBUG);
	}
}

PlayerConnection	*Server::search_client_by_name_nolock(const std::string& name) noexcept
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
	running(true),
	conn_window(0),
	conn_count(0)
{
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

const std::list<std::string>&		Server::get_banned_clients(void) const noexcept
{
	return (banned_clients);
}

void	Server::set_owner(ServerOwner *owner) noexcept
{
	this->owner = owner;
}

void	Server::set_world(World *world) noexcept
{
	this->world = world;
}

void	Server::set_running(bool running) noexcept
{
	this->running = running;
}

// Utils ----------------------------------------------------------------------

void				Server::start(void)
{
	if (on)
	{
		log("Tried to start server, but it is already on.", LogLevel::WARNING);
		return;
	}

	sock = init();
	if (sock == -1)
		throw ServerError("Server initialization error.");
	on = true;
	accept_thread = std::thread(&Server::accept_loop, this);
}

void				Server::stop(void)
{
	if (!on)
	{
		log("Tried to stop server, but it is already off.", LogLevel::WARNING);
		return;
	}

	// Shutting down the server...
	on = false;
	accept_thread.join();
	close(sock);
	sock = -1;

	// Setting the players as not connected...
	{
		std::lock_guard<std::mutex>	lock(clients_mtx);
		for (PlayerConnection& client: clients)
			client.set_connected(false);
	}

	// Joining client threads...
	for (std::thread& thread: thread_list)
		if (thread.joinable())
			thread.join();

	// Disconnecting players...
	{
		std::lock_guard<std::mutex>	lock(clients_mtx);
		for (PlayerConnection& client: clients)
			client.disconnect();
	}
}

void				Server::send_msg_to(int dst, const std::string& msg)
{
	PlayerConnection	*target;

	if (dst < 0)
		return;
	target = find_client_by_fd(dst);
	if (target)
		target->get_player().send_to_outbox(msg);	
}

void				Server::broadcast(const std::string& msg, int fd_excluded)
{
	std::lock_guard<std::mutex>	lock(clients_mtx);

	for (PlayerConnection& client: clients)
		if (client.get_client_fd() != fd_excluded)
			client.get_player().send_to_outbox(msg);
}

void				Server::group_broadcast(Group& group, const std::string& msg, int fd_excluded)
{
	std::lock_guard<std::mutex>	lock(groups_mtx);

	for (PlayerConnection *member: group.get_members())
		if (member->get_client_fd() != fd_excluded)
			member->get_player().send_to_outbox(msg);
}

void				Server::push_command(const t_command& cmd)
{
	std::lock_guard<std::mutex>	lock(cmd_mtx);

	cmd_queue.push_back(cmd);
}

void				Server::push_join(PlayerConnection* client)
{
	std::lock_guard<std::mutex>	lock(joins_mtx);
	pending_joins.push_back(client);
}

void				Server::push_leave(PlayerConnection *client)
{
	std::lock_guard<std::mutex>	lock(leaves_mtx);
	pending_leaves.push_back(client);
}

void				Server::game_loop(void)
{
	Command		cmd;
	t_command	cmd_info;
	bool		got_one;
	bool		joined;
	bool		left;

	while (running)
	{
		// Joining clients
		joined = false;
		{
			std::lock_guard<std::mutex>	lock(joins_mtx);

			for (PlayerConnection *client: pending_joins)
			{
				if (!client->get_player().get_current_room())
				{
					Player&	p = client->get_player();
					world->get_spawn_room()->add_player(&client->get_player());
					world->get_spawn_room()->room_broadcast(evt_presence_enter(p.get_name()), &p);
					joined = true;
				}
			}
			pending_joins.clear();
		}
		if (joined)
			broadcast(evt_stats(count_clients()));

		// Leaving clients
		left = false;
		{
			std::lock_guard<std::mutex>	lock(leaves_mtx);

			for (PlayerConnection *client: pending_leaves)
			{
				Player&	p = client->get_player();
				Room	*room = p.get_current_room();
				if (room)
				{
					room->room_broadcast(evt_presence_leave(p.get_name()), &p);
					room->remove_player(&p);
					left = true;
				}
			}
		}
		if (left)
			broadcast(evt_stats(count_clients()));

		// Reading if there is a command
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

		// Executing found command
		if (got_one && cmd_info.sender->is_connected())
		{
			log("Command from '" + cmd_info.sender->get_player().get_name() + "': " + cmd_info.text, LogLevel::DEBUG);
			if (cmd_info.sender->is_flooding())
				log("Possible command flooding from '" + cmd_info.sender->get_player().get_name() + "'", LogLevel::WARNING);
			assert(world != nullptr && "World cannot be nullptr.");
			try
			{
				cmd = CommandParser::parse(cmd_info.text);
				CommandHandler::handle(cmd, *cmd_info.sender, *this->world);
			}
			catch (const CommandParseError& e)
			{
				(*cmd_info.sender).get_player().send_to_outbox(e.what());
				log(e.what(), LogLevel::ERROR);
				continue;
			}
		}
		else
			std::this_thread::sleep_for(std::chrono::milliseconds(GAME_LOOP_MS_CD));
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

PlayerConnection	*Server::search_client_by_name(const std::string& name) noexcept
{
	std::lock_guard<std::mutex>	lock(clients_mtx);

	for (PlayerConnection& client: clients)
		if (client.get_player().get_name() == name)
			return (&client);
	return (nullptr);
}

Group				*Server::find_group_by_leader(PlayerConnection& leader) noexcept
{
	if (!leader.get_group())
		return (nullptr);

	std::lock_guard<std::mutex>	lock(groups_mtx);
	for (Group& group: groups)
		if (group.get_leader() == &leader)
			return (&group);
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
	Player						*player;
	std::list<std::string>		banned_clients_copy;
	bool						banned;

	result += bars;
	result += "\n";
	{
		// We make a copy so we forget about the mutex.
		std::lock_guard<std::mutex>	lock(banned_mtx);
		banned_clients_copy = banned_clients;
	}

	// Reading clients
	std::lock_guard<std::mutex>	lock(clients_mtx);
	if (clients.size() == 0)
		result += "None\n";
	for (PlayerConnection& client: clients)
	{
		// It is not possible to add in-game related information (data races).
		banned = false;
		player = &client.get_player();
		result += player->get_name() + ": ";
		if (client.is_connected())
			result += "Connected (fd: " + std::to_string(client.get_client_fd()) + ")\n";
		else
		{
			for (const std::string& client_banned: banned_clients_copy)
				if (client_banned == player->get_name())
					banned = true;
			if (banned)
				result += "Disconnected (Banned)\n";
			else
				result += "Disconnected\n";
		}
	}
	result += bars;
	return (result);
}

bool				Server::is_client_banned(const std::string& name) noexcept
{
	std::lock_guard<std::mutex>	lock(banned_mtx);

	for (const std::string& banned_name: banned_clients)
		if (banned_name == name)
			return (true);
	return (false);
}

bool				Server::ban_client(const std::string& name) noexcept
{
	PlayerConnection			*client;

	client = search_client_by_name(name);
	if (!client)
		return (false);	// Client not found.

	if (client->is_connected())
		client->set_connected(false);
	std::lock_guard<std::mutex>	lock_banned(banned_mtx);
	for (const std::string& banned: banned_clients)
		if (banned == name)
			return (false);	// Client is already banned.
	banned_clients.push_back(name);
	return (true);
}

std::string			Server::get_commands_instructions(void) const noexcept
{
	const std::string	bars = "=====";
	const std::string	connect_instructions = "CONNECT: Receives one argument, the client name.";
	const std::string	look_instructions = "LOOK: Takes no arguments. Gives all the information of the player's room in json format.";
	const std::string	move_instructions = "MOVE: Receives one argument, a direction. Moves the player in the specified direction.";
	const std::string	chat_instructions = "CHAT: Receives two arguments, the scope (GLOBAL, ROOM, GROUP), and the message to send.";
	const std::string	take_instructions = "TAKE: Receives one argument, the item to take.";
	const std::string	drop_instructions = "DROP: Receives one argument, the item to drop in the current room.";
	const std::string	inventory_instructions = "INVENTORY: Takes no arguments. Shows the player's current inventory.";
	const std::string	talk_instructions = "TALK: Receives one argument, the npc to interact with.";
	const std::string	open_instructions = "OPEN: Takes no arguments. Tries to open the chest in the current room.";
	const std::string	buy_instructions = "BUY: Receives one argument, the item to buy. The player will spend money if the item can be bought.";
	const std::string	equip_instructions = "EQUIP: Receives one argument, the item to equip. Only shields, armor and weapons are able to be equipped.";
	const std::string	enchant_instructions = "ENCHANT: Receives two arguments, the enchant to apply to the gear, and the gear. Applies that enchantment if it is possible";
	const std::string	fight_instructions = "FIGHT: Takes no arguments. Starts a fight with the current enemy in the room (if there is one). Until the fight is done, some commands become unusuable.";
	const std::string	attack_instructions = "ATTACK: Takes no arguments. Attacks the enemy in the current fight.";
	const std::string	defend_instructions = "DEFEND: Takes no arguments. Defends from the attack of the enemy in the current fight.";
	const std::string	flee_instructions = "FLEE: Takes no arguments. Runs away from the current fight, making the player to loose some money.";
	const std::string	consume_instructions = "CONSUME: Receives one argument, the consumable to consume in the current fight.";
	const std::string	status_instructions = "STATUS: Takes no arguments. Shows the player's status.";
	const std::string	quest_instructions = "QUEST: Receives one argument, the Quest Giver. Tries to request a quest in the current room.";
	const std::string	quests_instructions = "QUESTS: Takes no arguments. Shows the quest list of the player.";
	const std::string	who_instructions = "WHO: Takes no arguments. Shows how many connected players there are at the moment.";
	const std::string	group_instructions = "GROUP: Takes at least one argument. Depending on the scope (first argument), a second one might be needed. Supported scopes: CREATE, INVITE, JOIN, KICK, LEAVE.";
	const std::string	quit_instructions = "QUIT: Takes no arguments. Disconnects the client.";
	const std::string	help_instructions = "HELP: Takes no arguments. Shows this command list.";
	std::string			result;

	result += bars;
	result += '\n';
	result += connect_instructions;
	result += '\n';
	result += look_instructions;
	result += '\n';
	result += move_instructions;
	result += '\n';
	result += chat_instructions;
	result += '\n';
	result += take_instructions;
	result += '\n';
	result += drop_instructions;
	result += '\n';
	result += inventory_instructions;
	result += '\n';
	result += talk_instructions;
	result += '\n';
	result += open_instructions;
	result += '\n';
	result += buy_instructions;
	result += '\n';
	result += equip_instructions;
	result += '\n';
	result += enchant_instructions;
	result += '\n';
	result += fight_instructions;
	result += '\n';
	result += attack_instructions;
	result += '\n';
	result += defend_instructions;
	result += '\n';
	result += flee_instructions;
	result += '\n';
	result += consume_instructions;
	result += '\n';
	result += status_instructions;
	result += '\n';
	result += quest_instructions;
	result += '\n';
	result += quests_instructions;
	result += '\n';
	result += who_instructions;
	result += '\n';
	result += group_instructions;
	result += '\n';
	result += quit_instructions;
	result += '\n';
	result += help_instructions;
	result += '\n';
	result += bars;
	return (result);
}

size_t				Server::count_clients(void) noexcept
{
	std::lock_guard<std::mutex>	lock(clients_mtx);
	size_t	count;

	count = 0;
	for (PlayerConnection& client: clients)
		if (client.is_connected())
			count += 1;
	return (count);
}

Group				*Server::create_group(PlayerConnection& leader)
{
	if (leader.get_group())
		return (nullptr);

	std::lock_guard<std::mutex>	lock(groups_mtx);
	groups.emplace_back(leader);
	leader.set_group(&groups.back());
	return (&groups.back());
}

bool				Server::invite_group(PlayerConnection& inviter, PlayerConnection& target)
{
	if (!inviter.get_group())
		return (false);

	// Inviting target
	std::lock_guard<std::mutex>	lock(groups_mtx);
	if (!inviter.get_group()->invite_member(target))
		return (false);
	return (true);
}

Group				*Server::join_group(PlayerConnection& member, PlayerConnection& leader)
{
	Group						*target_group;
	
	target_group = nullptr;

	// Looking for target group
	{
		std::lock_guard<std::mutex>	lock(groups_mtx);
		for (Group& group: groups)
			if (group.get_leader() == &leader)
				target_group = &group;
	}
	if (!target_group)
		return (nullptr);
	if (target_group->accept_member(member))
		return (target_group);
	return (nullptr);
}

bool				Server::leave_group(PlayerConnection& member)
{
	Group	*group;

	group = member.get_group();
	if (!group)
		return (false);

	std::lock_guard<std::mutex>	lock(groups_mtx);
	if (!group->remove_member(member))
		return (false);
	if (group->get_members().empty())
		groups.remove(*group);
	return (true);
}

bool				Server::is_connection_flooding(void) noexcept
{
	std::time_t	now;

	now = std::time(nullptr);
	if (now - conn_window >= 1)
	{
		conn_window = now;
		conn_count = 0;
	}
	conn_count++;
	if (conn_count > MAX_CONNS_PER_SEC)
		return (true);
	return (false);
}

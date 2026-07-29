#pragma once
#include <atomic>
#include <mutex>
#include <sys/socket.h>
#include <thread>
#include <vector>

#include "server/PlayerConnection.hpp"
#include "world/World.hpp"

class ServerError final: public std::exception
{
	private:
		std::string					msg;

		static const std::string	DEFAULT_MSG;	// Defined in Server.cpp
	public:
		// Constructors -------------------------------------------------------

		ServerError(void);
		explicit ServerError(const std::string& msg);

		// Utils --------------------------------------------------------------

		const char	*what(void) const noexcept override;
};

/**
 * @brief	Struct that will be used for the server command list.
 * @param	sender	Pointer to the client that sent the command.
 * @param	text	The command sent by the client.
 */
struct t_command
{
	PlayerConnection	*sender;
	std::string			text;
};

class ServerOwner;

class Server
{
	private:
		int							sock;
		ServerOwner					*owner;
		World						*world;

		std::list<PlayerConnection>	clients;
		std::mutex					clients_mtx;

		std::list<t_command>		cmd_queue;
		std::mutex					cmd_mtx;

		std::atomic<bool>			on;
		std::thread					accept_thread;

		/**
		 * @brief	When shutting down the server, we need to know
		 * 			about all threads. This vector helps us that way.
		 * 			Mutex is not needed because before joining all of
		 * 			them accept_thread will be joined, so the list will
		 * 			not be modified anywhere.
		 */
		std::vector<std::thread>	thread_list;

		/**
		 * @brief	Variable used to check if the program is running.
		 */
		std::atomic<bool>			running;

		std::list<std::string>		banned_clients;
		std::mutex					banned_mtx;

		static constexpr int			DOMAIN = AF_INET;
		static constexpr int			TYPE = SOCK_STREAM;
		static constexpr int			DEFAULT_PORT = 8080;
		static constexpr unsigned int	MAX_CLIENTS = 20;
		static constexpr size_t			MAX_MSG_LENGTH = 1024;

		/**
		 * @brief	Initializes the server socket.
		 * @returns	The socket fd. -1 if any problem happened.
		 */
		int	init(void);

		/**
		 * @brief	Connects a new user or reconnects if it already exists
		 * 			to the given fd.
		 * @param	fd	The file descriptor the user will be using.
		 * @returns	A new account or one that already exists. `nullptr` if
		 * 			any error happens during the connection.
		 */
		PlayerConnection	*setup_client(const int fd);

		/**
		 * @brief	Thread that will simulate each client.	
		 */
		void				client_thread(int fd);

		/**
		 * @brief	Thread that will be accepting new clients.
		 */
		void				accept_loop(void);

		/**
		 * @note	This method will not lock `clients_mtx`.
		 */
		PlayerConnection	*search_client_by_name(const std::string& name) noexcept;

	public:
		// Constructors -------------------------------------------------------

		/**
		 * 
		 * @throws	`ServerError` if any problem during server initialization
		 * 			happened.
		 */
		Server(void);
		Server(const Server& server) = delete;
		~Server(void);

		// Operators ----------------------------------------------------------

		Server&	operator=(const Server& other) = delete;

		// Getters and setters ------------------------------------------------

		ServerOwner							*get_server_owner(void) const noexcept;
		World								*get_world(void) const noexcept;
		const std::list<PlayerConnection>&	get_clients(void) const noexcept;
		bool								is_on(void) const noexcept;
		const std::list<std::string>&		get_banned_clients(void) const noexcept;

		void	set_owner(ServerOwner *owner) noexcept;
		void	set_world(World *world) noexcept;

		/**
		 * @note	This method should only be used ONCE in the entire
		 * 			program, swapping the value to `false`.
		 */
		void	set_running(bool running) noexcept TAP_COLD;

		// Utils --------------------------------------------------------------

		/**
		 * @throws	`ServerError` if `init` failed.
		 */
		void	start(void);

		/**
		 * @brief	Turns the server off: Waits `accept_thread` to finish,
		 * 			closes the socket, sets it to -1, sets all clients as
		 * 			disconnected, and joins all client threads.
		 */
		void	stop(void);
		void	send_msg_to(int dst, const std::string& msg);

		/**
		 * @brief				Sends a message to all clients.
		 * @param	msg			The message to send.
		 * @param	fd_excluded	The file descriptor that won't receive this
		 * 						message. If `-1` , all clients will
		 * 						receive the message.
		 */
		void	broadcast(const std::string& msg, int fd_excluded = -1);

		/**
		 * @brief	Broadcasts that a client connected.
		 */
		void	announce_connection(PlayerConnection& client);

		/**
		 * @brief	Broadcasts that a client disconnected.
		 */
		void	announce_disconnection(PlayerConnection& client);

		void	push_command(const t_command& cmd);
		void	game_loop(void);

		PlayerConnection	*find_client_by_fd(int fd) noexcept;
		bool				is_fd_available(int fd) noexcept;

		/**
		 * @brief	Creates a string with all the client's information.
		 * @returns	The generated string.
		 */
		std::string			list_clients(void) noexcept;

		/**
		 * @brief	Checks if a player is in the bans list.
		 * @param	client	The player to check.
		 */
		bool				is_client_banned(const std::string& name) noexcept;

		/**
		 * @brief	Bans the client with the specified name from the server.
		 * @param	name	The name of the client to ban.
		 * @return	`true` if ban was succesfull. `false` otherwise.
		 */
		bool				ban_client(const std::string& name) noexcept;

		/**
		 * @brief	Creates a string with all the available commands and their
		 * 			information.
		 * @returns	A string with all commands instructions explained.
		 */
		std::string			get_commands_instructions(void) const noexcept;
};

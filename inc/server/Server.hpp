#pragma once
#include <mutex>
#include <sys/socket.h>

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
		std::list<PlayerConnection>	players;
		std::list<t_command>		command_queue;
		std::mutex					mtx;
		bool						on;

		// static constexpr int		DOMAIN = AF_INET;
		// static constexpr int		TYPE = SOCK_STREAM;
		static constexpr int			DEFAULT_PORT = 8080;
		static constexpr unsigned int	MAX_USERS = 20;

		/**
		 * @brief	Initializes the server socket.
		 * @returns	The socket fd. -1 if any problem happened.
		 */
		int	init(void);
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
		// std::list<PlayerConnection>&		get_players(void) noexcept;
		const std::list<PlayerConnection>&	get_players(void) const noexcept;
		bool								is_on(void) const noexcept;

		void	set_owner(ServerOwner *owner) noexcept;
		void	set_world(World *world) noexcept;

		// Utils --------------------------------------------------------------

		void	start(void);
		void	stop(void);
		void	send_msg_to(int dst, const std::string& msg);
		void	broadcast(const std::string& msg);
		void	connect_player(PlayerConnection& player);
		void	disconnect_player(PlayerConnection& player);
};

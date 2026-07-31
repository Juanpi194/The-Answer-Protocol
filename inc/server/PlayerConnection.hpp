#pragma once
#include <atomic>

#include "characters/Player.hpp"

class Group;
class Server;

class PlayerConnection final
{
	private:
		Player				player;
		std::atomic<int>	client_fd;
		std::atomic<bool>	connected;
		Server				*server;
		std::atomic<bool>	quitting;
		Group				*group;
	public:
		// Constructors -------------------------------------------------------

		PlayerConnection(const std::string& name, int client_fd, Server *server);
		PlayerConnection(const PlayerConnection& player) = delete;

		/** 
		 * @brief	Closes fd, sets it to -1, sets connected as false, ... 
		*/
		~PlayerConnection(void);

		// Operators ----------------------------------------------------------

		PlayerConnection&	operator=(const PlayerConnection& other) = delete;

		// Getters and setters ------------------------------------------------

		Player&			get_player(void) noexcept;
		const Player&	get_player(void) const noexcept;
		int				get_client_fd(void) const noexcept;
		bool			is_connected(void) const noexcept;
		Server			*get_server(void) const noexcept;
		bool			is_quitting(void) const noexcept;
		Group			*get_group(void) const noexcept;

		/**
		 * @note	`client_fd` lower than `0` is not allowed.
		 */
		void	set_client_fd(int client_fd) noexcept;
		void	set_connected(bool connected) noexcept;
		void	set_server(Server *server);
		void	set_quitting(bool quitting) noexcept;
		void	set_group(Group *group) noexcept;

		// Utils --------------------------------------------------------------

		/**
		 * @brief	Reconnects a player and sets the specified file descriptor
		 * 			as the new one.
		 * @param	fd	The new client's file descriptor.
		 */
		void		reconnect(int fd);

		/**
		 * @brief	Called when the player is disconnecting.
		 * 			Sets `connected` to false, closes the `client_fd`, and
		 * 			sets it to `-1`.
		 */
		void		disconnect(void);
};

#pragma once

#include "characters/Player.hpp"

class Server;

class PlayerConnection final
{
	private:
		Player	player;
		int		client_fd;
		bool	connected;
		Server	*server;
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

		void	set_client_fd(int client_fd);
		void	set_connected(bool connected) noexcept;
		void	set_server(Server *server);

		// Utils --------------------------------------------------------------

		void		connect(void);
		void		disconnect(void);
};

#pragma once
#include <string>

class Server;

class ServerOwner
{
	private:
		std::string	name;
		std::string	password;
		Server	*server;

		static constexpr bool	HARD_PASSWORD = true;
		static constexpr size_t	MIN_PASSWORD_LEN = 8;
		static constexpr size_t	MAX_PASSWORD_LEN = 20;
		static constexpr bool	UPPERCASE_NEEDED = true;
		static constexpr bool	LOWERCASE_NEEDED = true;
		static constexpr bool	NUMBERS_NEEDED = true;
		static constexpr bool	SYMBOLS_NEEDED = true;

		/**
		 * @brief	Asks the user to insert a valid password for the server.
		 * 			This password must follow the stated rules.
		 * @throws	`std::runtime_error` if Ctrl+D (`EOF`) is received.
		 * @returns	The validated password.
		 */
		std::string	ask_password(void);
	public:
		// Constructors -------------------------------------------------------

		/**
		 * @note	`name` format will not be checked here.
		*/
		ServerOwner(const std::string& name, Server *server);
		ServerOwner(const ServerOwner& server) = delete;
		~ServerOwner(void);

		// Operators ----------------------------------------------------------

		ServerOwner&	operator=(const ServerOwner& other) = delete;

		// Getters and setters ------------------------------------------------

		std::string	get_name(void) const noexcept;
		Server		*get_server(void) const noexcept;

		void	set_name(const std::string& name);
		void	set_server(Server *server);

		// Utils --------------------------------------------------------------

		void	change_password(void);
		void	init_server(void);
		void	shutdown_server(void);
		void	reset_server(void);
};

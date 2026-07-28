#pragma once
#include <string>

#include "utils/attributes.hpp"

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

		/**
		 * @brief	Handles command sent by the owner.
		 * @param	cmd	The command to execute.
		 * @returns	`false` if `cmd` is QUIT, `true` otherwise.
		 */
		bool		handle_cmd(const std::string& cmd) TAP_UNUSED_RESULT;

		// Methods called by the command handler --
		
		void		exit_server(void);
		void		init_server(void) noexcept;
		
		/**
		 * @brief	Completely stops the server
		 * 			(closing fd, kicking players, ...).
		 */
		void		shutdown_server(void);
		void		reset_server(void);
		void		list_server_clients(void);
		void		change_password(void);

		/**
		 * @brief	Short instructions for the supported commands.
		 */
		std::string	get_commands_instructions(void) const noexcept TAP_UNUSED_RESULT;
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

		/**
		 * @brief	The thread that will simulate the owner, working
		 * 			with the `std::cout`.
		 */
		void	owner_thread(void);
};

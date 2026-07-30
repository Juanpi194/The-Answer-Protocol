#include "server/ServerOwner.hpp"

#include <iostream>
#include <string>
#include <cctype>
#include <unistd.h>

#include "utils/utils.hpp"
#include "server/Server.hpp"

/**
 * @brief	Shows a message explaining why the given password is incorrect.
 * 			Resets `password` using the `clear` function.
 * @param	reason		The reason of why the password was rejected.
 * @param	detail		The detail behind the reason.
 * @param	password	The password to be reset.
*/
static void	reject_password(const std::string& reason, const std::string& detail, std::string& password)
{
	std::cout << "\nIncorrect password format!\n" << reason;
	if (!detail.empty())
		std::cout << detail << std::endl;
	std::cout << "Try again...\n" << std::endl;
	password.clear();
}

std::string	ServerOwner::ask_password(void)
{
	std::string	password;

	while (true)
	{
		std::cout << "Insert the new password for your server ('q' to exit if one already exists): ";
		std::getline(std::cin, password);
		if (std::cin.eof())
			throw std::runtime_error("Server owner did not introduce a password!");
		if (password == "q" && !this->password.empty())
			return (this->password);
		if (!HARD_PASSWORD)
			return (password);
		if (password.size() < MIN_PASSWORD_LEN)
		{
			reject_password("Password minimum length is: ", std::to_string(MIN_PASSWORD_LEN), password);
			continue;
		}
		if (password.size() > MAX_PASSWORD_LEN)
		{
			reject_password("Password maximum length is: ", std::to_string(MAX_PASSWORD_LEN), password);
			continue;
		}
		if (UPPERCASE_NEEDED && !has_uppercase(password))
		{
			reject_password("At least one character must be uppercase.", "", password);
			continue;
		}
		if (LOWERCASE_NEEDED && !has_lowercase(password))
		{
			reject_password("At least one character must be lowercase.", "", password);
			continue;
		}
		if (NUMBERS_NEEDED && !has_number(password))
		{
			reject_password("At least one character must be a digit.", "", password);
			continue;
		}
		if (SYMBOLS_NEEDED && !has_symbol(password))
		{
			reject_password("At least one character must be a symbol.", "", password);
			continue;
		}
		break;
	}
	return (password);
}

bool	ServerOwner::handle_cmd(const std::string& cmd)
{
	// TODO: Parse cmd.

	if (cmd == "EXIT")
	{
		if (confirm_password())
			return (exit_server(), false);
	}
	else if (cmd == "INIT")
	{
		if (confirm_password())
			init_server();
	}
	else if (cmd == "SHUTDOWN")
	{
		if (confirm_password())
			shutdown_server();
	}
	else if (cmd == "RESET")
	{
		if (confirm_password())
			reset_server();
	}
	else if (cmd == "LIST")
		list_server_clients();
	else if (cmd == "PWDCHANGE")
	{
		if (confirm_password())
			change_password();
	}
	else if (cmd == "BAN")
		std::cout << "'BAN' requires an argument. Usage: BAN <name>." << std::endl;
	else if (starts_with(cmd, "BAN "))
	{
		if (confirm_password())
			ban_client(cmd.substr(4));
	}
	else if (cmd == "HELP")
		std::cout << get_server_management_commands_instructions() << std::endl;
	else
		std::cout << "Invalid command. Write 'HELP' for commands information." << std::endl;
	return (true);
}

bool	ServerOwner::confirm_password(void) noexcept
{
	std::string	answer;

	std::cout << "Introduce password to execute action: ";
	std::getline(std::cin, answer);
	if (answer != password)
	{
		std::cout << "Wrong password. Action denied." << std::endl;
		return (false);
	}
	return (true);
}

void	ServerOwner::exit_server(void)
{
	if (!server)
		return;

	shutdown_server();
	server->set_running(false);
}

void	ServerOwner::init_server(void) noexcept
{
	if (!server || server->is_on())
		return;

	std::cout << "Initializing server..." << std::endl;	
	try
	{
		server->start();
		log("Server initiated.", LogLevel::INFO);
	}
	catch (const ServerError& e)
	{
		std::cout << e.what() << std::endl;
		log("Server initialization failed.", LogLevel::ERROR);
	}
}

void	ServerOwner::shutdown_server(void)
{
	if (!server || !server->is_on())
		return;

	std::cout << "Shutting down server..." << std::endl;
	server->stop();
	log("Server closed.", LogLevel::INFO);
}

void	ServerOwner::reset_server(void)
{
	std::string	answer;

	if (!server)
		return;

	std::cout << "Introduce password to execute action: ";
	std::getline(std::cin, answer);
	if (answer != password)
	{
		std::cout << "Wrong password. Action denied." << std::endl;
		return;
	}
	std::cout << "Resetting server..." << std::endl;
	shutdown_server();
	init_server();
}

void	ServerOwner::list_server_clients(void)
{
	if (!server)
		return;

	std::cout << server->list_clients() << std::endl;
}

void	ServerOwner::ban_client(std::string name)
{
	if (!server)
		return;

	trim_str(name, false);
	if (name.empty())
		std::cout << "Specified name is empty. 'BAN' requires a name. Usage: BAN <name>." << std::endl;
	else if (!server->ban_client(name))
		std::cout << "Client not found or already banned." << std::endl;
	else
		std::cout << ("'" + name + "' banned.") << std::endl;
}

void	ServerOwner::change_password(void)
{
	log("Request to change password received.", LogLevel::INFO);
	try
	{
		password = ask_password();
		std::cout << "Password successfully created!" << std::endl;
	}
	catch (const std::runtime_error& e)
	{
		std::cout << "\n" << std::endl;
		std::cout << e.what() << std::endl;
		std::cout << "The password did not change." << std::endl;
	}
}

std::string	ServerOwner::get_server_management_commands_instructions(void) const noexcept
{
	const std::string	bars = "=====";
	const std::string	init_instructions = "INIT: Starts the server.";
	const std::string	shutdown_instructions = "SHUTDOWN: Stops the server.";
	const std::string	reset_instructions = "RESET: Stops the server, then starts it.";
	const std::string	list_clients_instructions = "LIST: Lists all clients and their information.";
	const std::string	ban_client_instructions = "BAN: Bans the client with the specified name. Usage: 'BAN <name>'";
	const std::string	password_change_instructions = "PWDCHANGE: Changes the password.";
	const std::string	exit_instructions = "EXIT: Stops the server and exits the program.";
	const std::string	help_instructions = "HELP: Shows all the available commands.";
	std::string			commands;

	commands += bars;
	commands += '\n';
	commands += init_instructions;
	commands += '\n';
	commands += shutdown_instructions;
	commands += '\n';
	commands += list_clients_instructions;
	commands += '\n';
	commands += ban_client_instructions;
	commands += '\n';
	commands += password_change_instructions;
	commands += '\n';
	commands += exit_instructions;
	commands += '\n';
	commands += help_instructions;
	commands += '\n';
	commands += bars;
	return (commands);
}

// Constructors ---------------------------------------------------------------

ServerOwner::ServerOwner(const std::string& name, Server *server)
{
	const std::string	default_password = "Default123&";

	this->name = name;
	this->server = server;
	if (!server)
		log("No server was given to " + name, LogLevel::WARNING);
	std::cout << "Welcome, " << name << "!" << std::endl;
	std::cout << "From now on, you will be in charge of a server." << std::endl;
	std::cout << "Here you have all the available commands:" << std::endl;
	std::cout << get_server_management_commands_instructions() << std::endl;;
	std::cout << "Good luck :)" << std::endl;
	try
	{
		password = ask_password();
		std::cout << "Password successfully created!" << std::endl;
	}
	catch (const std::runtime_error& e)
	{
		std::cout << "\n" << std::endl;
		std::cout << e.what() << std::endl;
		std::cout << "Server's password will be set to '" << default_password << "'." << std::endl;
		password = default_password;
	}
}

// Getters and setters --------------------------------------------------------

std::string	ServerOwner::get_name(void) const noexcept
{
	return (name);
}

Server		*ServerOwner::get_server(void) const noexcept
{
	return (server);
}

void		ServerOwner::set_name(const std::string& name)
{
	std::string	temp = name;

	trim_str(temp);
	if (!temp.empty())
		this->name = name;
	else
		log("New name is empty and will not be established.", LogLevel::ERROR);
}

void		ServerOwner::set_server(Server *server)
{
	if (!server)
		log("Server given to " + name + " is nullptr.", LogLevel::WARNING);
	this->server = server;
}

// Utils ----------------------------------------------------------------------

void	ServerOwner::owner_thread(void)
{
	std::string	msg;

	while (true)
	{
		std::getline(std::cin, msg);
		if (std::cin.eof())
		{
			// This is to check if its redirected.
			// if (!isatty(STDIN_FILENO))
			// 	break;
			// std::cin.clear();
			// std::cout << "Cannot close the server owner console. Use EXIT." << std::endl;
			exit_server();
			break;
		}
		else if (std::cin.fail())
			std::cout << "Error reading input. Try again." << std::endl;
		else if (!handle_cmd(msg))
			break;
	}
}

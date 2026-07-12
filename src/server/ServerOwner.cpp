#include "server/ServerOwner.hpp"

#include <iostream>
#include <string>
#include <cctype>

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
		std::cout << "Insert the new password for your server: ";
		std::cin >> password;
		if (std::cin.eof())
			throw std::runtime_error("Server owner did not introduce a password!");
		if (!HARD_PASSWORD)
			return (password);
		if (password.size() < MIN_PASSWORD_LEN)
		{
			reject_password("Password minimum length is: ", std::to_string(MIN_PASSWORD_LEN), password);
			continue ;
		}
		if (password.size() > MAX_PASSWORD_LEN)
		{
			reject_password("Password maximum length is: ", std::to_string(MAX_PASSWORD_LEN), password);
			continue ;
		}
		if (UPPERCASE_NEEDED && !has_uppercase(password))
		{
			reject_password("At least one character must be uppercase.", "", password);
			continue ;
		}
		if (LOWERCASE_NEEDED && !has_lowercase(password))
		{
			reject_password("At least one character must be lowercase.", "", password);
			continue ;
		}
		if (NUMBERS_NEEDED && !has_number(password))
		{
			reject_password("At least one character must be a digit.", "", password);
			continue ;
		}
		if (SYMBOLS_NEEDED && !has_symbol(password))
		{
			reject_password("At least one character must be a symbol.", "", password);
			continue ;
		}
		break ;
	}
	return (password);
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
	std::cout << "Good luck :)" << std::endl;
	try
	{
		password = ask_password();
	}
	catch (const std::runtime_error& e)
	{
		std::cout << "\n" << std::endl;
		std::cout << e.what() << std::endl;
		std::cout << "Server's password will be set to '" << default_password << "'." << std::endl;
		password = default_password;
	}
}

ServerOwner::~ServerOwner(void)
{
	// if (server)
		// close
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

void	ServerOwner::change_password(void)
{
	log("Request to change password received.", LogLevel::INFO);
	try
	{
		password = ask_password();
	}
	catch (const std::runtime_error& e)
	{
		std::cout << "\n" << std::endl;
		std::cout << e.what() << std::endl;
		std::cout << "The password did not change." << std::endl;
	}
}

void	ServerOwner::init_server(void)
{
	if (!server || server->is_on())
		return ;

	// TODO: Try catch
	server->start();
	log("Server initiated.", LogLevel::INFO);
}

void	ServerOwner::shutdown_server(void)
{
	if (!server || !server->is_on())
		return ;

	// TODO: Try catch
	server->stop();
	log("Server closed.", LogLevel::INFO);
}

void	ServerOwner::reset_server(void)
{
	if (!server)
		return ;

	// TODO: Try catch
	shutdown_server();
	init_server();
	log("Server reset.", LogLevel::INFO);
}

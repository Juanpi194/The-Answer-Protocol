#include "commands/commandparser.hpp"

#include <cctype>

#include "utils/utils.hpp"

static const char *CHAT_CHANNELS[] =
{
	"GLOBAL",
	"ROOM",
	"GROUP"
};

static const char *GROUP_SUBCOMMANDS[] =
{
	"INVITE",
	"LEAVE",
	"KICK"
};

static void	split_first_token(const std::string& str,
	std::string& first,
	std::string& rest)
{
	size_t	pos = str.find(' ');

	if (pos == std::string::npos)
	{
		first = str;
		rest = "";
	}
	else
	{
		first = str.substr(0, pos);
		rest = str.substr(pos + 1);
	}
}
//Separa la primera palabra del resto de la línea (por ejemplo, el comando de sus argumentos).

static bool	is_one_of(const std::string& value,
	const char *const options[],
	size_t count)
{
	for (size_t i = 0; i < count; ++i)
	{
		if (value == options[i])
			return (true);
	}
	return (false);
}
//Comprueba si una cadena coincide con alguno de los valores permitidos de una lista.

static Command	parse_no_arguments(CommandType type,
	const std::string& rest,
	const std::string& name)
{
	if (!rest.empty())
		throw CommandParseError(name + " does not take any arguments.");
	return (Command{type, {}});
}
//Valida comandos que no aceptan argumentos y crea el Command.

static Command	parse_text_argument(CommandType type,
	const std::string& rest,
	const std::string& name)
{
	if (rest.empty())
		throw CommandParseError(name + " requires an argument.");
	return (Command{type, {rest}});
}
//Valida comandos que requieren un único argumento de texto libre (puede contener espacios).

static Command	parse_single_argument(CommandType type,
	const std::string& rest,
	const std::string& name)
{
	if (rest.empty())
		throw CommandParseError(name + " requires an argument.");

	if (rest.find(' ') != std::string::npos)
		throw CommandParseError(name + " takes exactly one argument.");

	return (Command{type, {rest}});
}
//Valida comandos que requieren exactamente un argumento sin espacios

static Command	parse_attack(const std::string& rest)
{
	if (rest.empty())
		return (Command{CommandType::ATTACK, {}});

	return (Command{CommandType::ATTACK, {rest}});
}
//Parsea el comando ATTACK, permitiendo usarlo con o sin objetivo.

static Command	parse_chat(const std::string& rest)
{
	std::string	channel;
	std::string	message;

	split_first_token(rest, channel, message);

	channel = to_upper(channel);

	if (!is_one_of(channel, CHAT_CHANNELS, 3))
		throw CommandParseError(
			"CHAT channel must be GLOBAL, ROOM or GROUP.");

	if (message.empty())
		throw CommandParseError("CHAT requires a message.");

	return (Command{CommandType::CHAT, {channel, message}});
}
//Valida el canal de CHAT y extrae el mensaje que se quiere enviar.

static Command	parse_group(const std::string& rest)
{
	std::string	subcommand;
	std::string	target;

	split_first_token(rest, subcommand, target);

	subcommand = to_upper(subcommand);

	if (subcommand.empty())
		return (Command{CommandType::GROUP, {}});

	if (!is_one_of(subcommand, GROUP_SUBCOMMANDS, 3))
	{
		throw CommandParseError(
			"GROUP subcommand must be INVITE, LEAVE or KICK.");
	}

	if (subcommand == "LEAVE")
	{
		if (!target.empty())
		{
			throw CommandParseError(
				"GROUP LEAVE does not take any arguments.");
		}
		return (Command{CommandType::GROUP, {subcommand}});
	}

	if (target.empty())
	{
		throw CommandParseError(
			"GROUP " + subcommand + " requires a target player.");
	}

	if (target.find(' ') != std::string::npos)
	{
		throw CommandParseError(
			"GROUP " + subcommand +
			" takes exactly one target player.");
	}

	return (Command{CommandType::GROUP, {subcommand, target}});
}
//Valida los subcomandos de GROUP (INVITE, LEAVE o KICK) y sus argumentos

Command	CommandParser::parse(const std::string& line)
{
	std::string	trimmed = line;
	std::string	keyword;
	std::string	rest;

	trim_str(trimmed);

	if (trimmed.empty())
		throw CommandParseError("Empty command line.");

	split_first_token(trimmed, keyword, rest);

	keyword = to_upper(keyword);

	// Commands without arguments
	if (keyword == "LOOK")
		return (parse_no_arguments(CommandType::LOOK, rest, keyword));

	if (keyword == "INVENTORY")
		return (parse_no_arguments(CommandType::INVENTORY, rest, keyword));

	if (keyword == "STATUS")
		return (parse_no_arguments(CommandType::STATUS, rest, keyword));

	if (keyword == "QUESTS")
		return (parse_no_arguments(CommandType::QUESTS, rest, keyword));

	if (keyword == "WHO")
		return (parse_no_arguments(CommandType::WHO, rest, keyword));

	if (keyword == "QUIT")
		return (parse_no_arguments(CommandType::QUIT, rest, keyword));

	// Commands with a single argument
	if (keyword == "CONNECT")
		return (parse_single_argument(CommandType::CONNECT, rest, keyword));

	if (keyword == "MOVE")
		return (parse_single_argument(CommandType::MOVE, rest, keyword));

	if (keyword == "QUEST")
		return (parse_single_argument(CommandType::QUEST, rest, keyword));

	// Commands with free text
	if (keyword == "TAKE")
		return (parse_text_argument(CommandType::TAKE, rest, keyword));

	if (keyword == "DROP")
		return (parse_text_argument(CommandType::DROP, rest, keyword));

	if (keyword == "TALK")
		return (parse_text_argument(CommandType::TALK, rest, keyword));

	// Special commands
	if (keyword == "ATTACK")
		return (parse_attack(rest));

	if (keyword == "CHAT")
		return (parse_chat(rest));

	if (keyword == "GROUP")
		return (parse_group(rest));

	throw CommandParseError("Unknown command '" + keyword + "'.");
}
//Analiza una línea del protocolo, identifica el comando y llama al parser específico correspondiente.
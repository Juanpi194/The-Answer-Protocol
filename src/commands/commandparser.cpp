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
	"KICK",
	"CREATE",
	"JOIN"
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
// Exige 0 argumentos. Usado por LOOK, INVENTORY, STATUS, QUESTS, WHO, QUIT, ATTACK, DEFEND, FLEE, FIGHT.
static Command	parse_no_arguments(CommandType type,
	const std::string& rest)
{
	if (!rest.empty())
		throw CommandParseError(ErrorCode::WRONG_ARGUMENTS);
	return (Command{type, {}});
}
//Valida comandos que no aceptan argumentos y crea el Command.
// Exige 1 argumento de texto libre (puede tener espacios). Usado por TAKE, DROP, TALK.
static Command	parse_text_argument(CommandType type,
	const std::string& rest)
{
	if (rest.empty())
		throw CommandParseError(ErrorCode::WRONG_ARGUMENTS);
	return (Command{type, {rest}});
}
//Valida comandos que requieren un único argumento de texto libre (puede contener espacios).
// Exige 1 argumento, sin espacios (una única palabra). Usado por CONNECT, MOVE, QUEST, CONSUME.
static Command	parse_single_argument(CommandType type,
	const std::string& rest)
{
	if (rest.empty())
		throw CommandParseError(ErrorCode::WRONG_ARGUMENTS);

	if (rest.find(' ') != std::string::npos)
		throw CommandParseError(ErrorCode::WRONG_ARGUMENTS);

	return (Command{type, {rest}});
}
//Valida comandos que requieren exactamente un argumento sin espacios
// Exige 2 argumentos: canal (GLOBAL/ROOM/GROUP) y mensaje. Usado por CHAT.
static Command	parse_chat(const std::string& rest)
{
	std::string	channel;
	std::string	message;

	split_first_token(rest, channel, message);

	channel = to_upper(channel);

	if (!is_one_of(channel, CHAT_CHANNELS, 3))
		throw CommandParseError(ErrorCode::INVALID_ARGUMENT);

	if (message.empty())
		throw CommandParseError(ErrorCode::WRONG_ARGUMENTS);

	return (Command{CommandType::CHAT, {channel, message}});
}
//Valida el canal de CHAT y extrae el mensaje que se quiere enviar.
// Exige 0, 1 o 2 argumentos según el subcomando (INVITE/LEAVE/KICK/CREATE). Usado por GROUP.
static Command	parse_group(const std::string& rest)
{
	std::string	subcommand;
	std::string	target;

	split_first_token(rest, subcommand, target);

	subcommand = to_upper(subcommand);

	if (subcommand.empty())
		return (Command{CommandType::GROUP, {}});

	if (!is_one_of(subcommand, GROUP_SUBCOMMANDS, 5))
		throw CommandParseError(ErrorCode::INVALID_ARGUMENT);

	if (subcommand == "LEAVE" || subcommand == "CREATE")
	{
		if (!target.empty())
			throw CommandParseError(ErrorCode::WRONG_ARGUMENTS);
		return (Command{CommandType::GROUP, {subcommand}});
	}

	if (target.empty())
		throw CommandParseError(ErrorCode::WRONG_ARGUMENTS);

	if (target.find(' ') != std::string::npos)
		throw CommandParseError(ErrorCode::WRONG_ARGUMENTS);

	return (Command{CommandType::GROUP, {subcommand, target}});
}
//Valida los subcomandos de GROUP (INVITE, LEAVE, KICK, CREATE o JOIN) y sus argumentos

// Exige 2 argumentos: encantamiento (una palabra) y el item a encantar (texto libre). Usado por ENCHANT.
static Command	parse_enchant(const std::string& rest)
{
	std::string	enchantment;
	std::string	item;

	split_first_token(rest, enchantment, item);

	if (enchantment.empty() || item.empty())
		throw CommandParseError(ErrorCode::WRONG_ARGUMENTS);

	return (Command{CommandType::ENCHANT, {enchantment, item}});
}
//Separa el encantamiento (primera palabra) del item a encantar (resto de la línea).

Command	CommandParser::parse(const std::string& line)
{
	std::string	trimmed = line;
	std::string	keyword;
	std::string	rest;

	trim_str(trimmed);

	if (trimmed.empty())
		throw CommandParseError(ErrorCode::INVALID_COMMAND);

	split_first_token(trimmed, keyword, rest);

	keyword = to_upper(keyword);

	if (keyword == "LOOK")
		return (parse_no_arguments(CommandType::LOOK, rest));

	if (keyword == "INVENTORY")
		return (parse_no_arguments(CommandType::INVENTORY, rest));

	if (keyword == "STATUS")
		return (parse_no_arguments(CommandType::STATUS, rest));

	if (keyword == "QUESTS")
		return (parse_no_arguments(CommandType::QUESTS, rest));

	if (keyword == "WHO")
		return (parse_no_arguments(CommandType::WHO, rest));

	if (keyword == "HELP")
		return (parse_no_arguments(CommandType::HELP, rest));

	if (keyword == "QUIT")
		return (parse_no_arguments(CommandType::QUIT, rest));

	if (keyword == "ATTACK")
		return (parse_no_arguments(CommandType::ATTACK, rest));

	if (keyword == "DEFEND")
		return (parse_no_arguments(CommandType::DEFEND, rest));

	if (keyword == "FLEE")
		return (parse_no_arguments(CommandType::FLEE, rest));

	if (keyword == "FIGHT")
		return (parse_no_arguments(CommandType::FIGHT, rest));

	if (keyword == "CONNECT")
		return (parse_single_argument(CommandType::CONNECT, rest));

	if (keyword == "MOVE")
		return (parse_single_argument(CommandType::MOVE, rest));

	if (keyword == "QUEST")
		return (parse_single_argument(CommandType::QUEST, rest));

	if (keyword == "CONSUME")
		return (parse_single_argument(CommandType::CONSUME, rest));

	if (keyword == "TAKE")
		return (parse_text_argument(CommandType::TAKE, rest));

	if (keyword == "DROP")
		return (parse_text_argument(CommandType::DROP, rest));

	if (keyword == "TALK")
		return (parse_text_argument(CommandType::TALK, rest));

	if (keyword == "OPEN")
		return (parse_no_arguments(CommandType::OPEN, rest));

	if (keyword == "BUY")
		return (parse_text_argument(CommandType::BUY, rest));

	if (keyword == "EQUIP")
		return (parse_text_argument(CommandType::EQUIP, rest));

	if (keyword == "ENCHANT")
		return (parse_enchant(rest));

	if (keyword == "CHAT")
		return (parse_chat(rest));

	if (keyword == "GROUP")
		return (parse_group(rest));

	throw CommandParseError(ErrorCode::INVALID_COMMAND);
}
//Analiza una línea del protocolo, identifica el comando y llama al parser específico correspondiente.

#include "commands/command.hpp"

CommandParseError::CommandParseError(const std::string& msg)
	: msg(msg)
{
}

const char *CommandParseError::what(void) const noexcept //la función devuelve un puntero a un char, es decir, una cadena de texto en formato C (const char*. noexcept → indica que esa función garantiza que no lanzará excepciones (throw); si lo hace, el programa termina.
{
	return (msg.c_str());
}

std::string	command_type_to_string(CommandType type)
{
	switch (type)
	{
		case CommandType::CONNECT:
			return ("CONNECT");
		case CommandType::LOOK:
			return ("LOOK");
		case CommandType::MOVE:
			return ("MOVE");
		case CommandType::CHAT:
			return ("CHAT");
		case CommandType::TAKE:
			return ("TAKE");
		case CommandType::DROP:
			return ("DROP");
		case CommandType::INVENTORY:
			return ("INVENTORY");
		case CommandType::TALK:
			return ("TALK");
		case CommandType::ATTACK:
			return ("ATTACK");
		case CommandType::DEFEND:
			return ("DEFEND");
		case CommandType::FLEE:
			return ("FLEE");
		case CommandType::CONSUME:
			return ("CONSUME");
		case CommandType::FIGHT:
			return ("FIGHT");
		case CommandType::STATUS:
			return ("STATUS");
		case CommandType::QUEST:
			return ("QUEST");
		case CommandType::QUESTS:
			return ("QUESTS");
		case CommandType::WHO:
			return ("WHO");
		case CommandType::GROUP:
			return ("GROUP");
		case CommandType::QUIT:
			return ("QUIT");
	}
	return ("UNKNOWN");
}
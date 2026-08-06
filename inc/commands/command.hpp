#pragma once
#include <exception>
#include <string>
#include <vector>

#include "protocol/responses.hpp"

enum class CommandType
{
	CONNECT,
	LOOK,
	MOVE,
	CHAT,
	TAKE,
	DROP,
	INVENTORY,
	TALK,
	OPEN,
	BUY,
	EQUIP,
	ENCHANT,
	FIGHT,
	ATTACK,
	DEFEND,
	FLEE,
	CONSUME,
	STATUS,
	QUEST,
	QUESTS,
	WHO,
	GROUP,
	QUIT,
	HELP
};

struct Command
{
	CommandType					type;
	std::vector<std::string>	args;
};

class CommandParseError : public std::exception
{
	private:
		std::string	msg;

	public:
		explicit CommandParseError(ErrorCode err_code);

		const char	*what(void) const noexcept override;
};

std::string	command_type_to_string(CommandType type);

#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <exception>
#include <string>
#include <vector>

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
	ATTACK,
	STATUS,
	QUEST,
	QUESTS,
	WHO,
	GROUP,
	QUIT
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
		explicit CommandParseError(const std::string& msg);

		const char	*what(void) const noexcept override;
};

std::string	command_type_to_string(CommandType type);

#endif
#pragma once
<<<<<<< HEAD
=======

>>>>>>> juanpi
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
	DEFEND,
	FLEE,
	CONSUME,
	FIGHT,
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

<<<<<<< HEAD
std::string	command_type_to_string(CommandType type);
=======
std::string	command_type_to_string(CommandType type);
>>>>>>> juanpi

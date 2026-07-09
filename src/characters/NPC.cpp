#include "characters/NPC.hpp"

#include <stdexcept>

#include "utils/utils.hpp"

unsigned int		NPC::available_id = 0;
const std::string	NPC::PREFIX = "npc.";

bool	NPC::validate_arguments(const std::string& name, const std::string& description)
{
	std::string	npc_name;
	std::string	npc_description;

	// NAME
	npc_name = name;
	trim_str(npc_name);
	if (npc_name.empty())
		return (log("NPC name cannot be empty.", LogLevel::ERROR), false);
	if (TITLE_NAME && !is_title(npc_name))
		return (log("NPC name is not a title.", LogLevel::ERROR), false);
	if (npc_name.size() > MAX_NAME_LENGTH)
		return (log("NPC name is too long (MAX CHARACTERS: " + std::to_string(MAX_NAME_LENGTH) + ").", LogLevel::ERROR), false);
	if (npc_name.size() < MIN_NAME_LENGTH)
		return (log("NPC name is too short (MIN CHARACTERS: " + std::to_string(MIN_NAME_LENGTH) + ").", LogLevel::ERROR), false);

	// DESCRIPTION
	npc_description = description;
	trim_str(npc_description);
	if (npc_description.empty())
		return (log("Description cannot be empty.", LogLevel::ERROR), false);
	if (npc_description.size() > MAX_DESCRIPTION_LENGTH)
		return (log("NPC description is too long (MAX CHARACTERS: " + std::to_string(MAX_DESCRIPTION_LENGTH) + ").", LogLevel::ERROR), false);
	if (npc_description.size() < MIN_DESCRIPTION_LENGTH)
		return (log("NPC description is too short (MIN CHARACTERS: " + std::to_string(MIN_DESCRIPTION_LENGTH) + ").", LogLevel::ERROR), false);

	// VALIDATION PASSED
	return (true);
}

// Constructors ---------------------------------------------------------------

NPC::NPC(const std::string& name, const std::string& description):
	Character(name),
	id(PREFIX + std::to_string(available_id++)),
	description(description)
{
	if (!validate_arguments(name, description))
		throw std::invalid_argument("NPC validation failed.");
}

NPC::NPC(const NPC& npc):
	Character(npc.get_name()),
	id(PREFIX + std::to_string(available_id++)),
	description(npc.description)
{
}

// Getters and setters --------------------------------------------------------

std::string	NPC::get_id(void) const noexcept
{
	return (id);
}

std::string	NPC::get_description(void) const noexcept
{
	return (description);
}

// Utils ----------------------------------------------------------------------

// TODO: Add utils functions

#include "quests/Quest.hpp"

#include <stdexcept>

#include "utils/utils.hpp"

// Constructors ---------------------------------------------------------------

Quest::Quest(const std::string& name, const std::string& description):
	name(name),
	description(description),
	completed(false)
{
	std::string	temp = name;

	// Name
	trim_str(temp);
	if (temp.empty())
		throw std::invalid_argument("Quest name cannot be empty.");
	if (temp.size() < MIN_NAME_LENGTH)
		throw std::invalid_argument("Quest name is too short.");
	if (temp.size() > MAX_NAME_LENGTH)
		throw std::invalid_argument("Quest name is too long.");

	// Description
	temp = description;
	trim_str(temp);
	if (temp.empty())
		throw std::invalid_argument("Quest description cannot be empty.");
	if (temp.size() < MIN_DESCRIPTION_LENGTH)
		throw std::invalid_argument("Quest description is too short.");
	if (temp.size() > MAX_DESCRIPTION_LENGTH)
		throw std::invalid_argument("Quest description is too long.");
}

Quest::Quest(const Quest& quest):
	name(quest.name),
	description(quest.description),
	completed(quest.completed)
{
}

// Getters and setters --------------------------------------------------------

std::string	Quest::get_name(void) const noexcept
{
	return (name);
}

std::string	Quest::get_description(void) const noexcept
{
	return (description);
}

bool		Quest::is_completed(void) const noexcept
{
	return (completed);
}

void	Quest::set_completed(bool completed) noexcept
{
	this->completed = completed;
}

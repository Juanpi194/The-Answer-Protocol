#include "quests/Quest.hpp"

#include <stdexcept>

#include "utils/utils.hpp"
#include "characters/Player.hpp"
#include "items/Item.hpp"

// Constructors ---------------------------------------------------------------

Quest::Quest(const std::string& name, const std::string& description, unsigned int gold_reward, Item *item_reward, const t_quest_objective& objective):
	name(name),
	description(description),
	completed(false),
	gold_reward(gold_reward),
	item_reward(item_reward),
	objective(objective)
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

std::string					Quest::get_name(void) const noexcept
{
	return (name);
}

std::string					Quest::get_description(void) const noexcept
{
	return (description);
}

bool						Quest::is_completed(void) const noexcept
{
	return (completed);
}

unsigned int				Quest::get_gold_reward(void) const noexcept
{
	return (gold_reward);
}

Item						*Quest::get_item_reward(void) const noexcept
{
	return (item_reward);
}

const t_quest_objective&	Quest::get_objective(void) const noexcept
{
	return (objective);
}

void	Quest::set_completed(bool completed) noexcept
{
	this->completed = completed;
}

// Utils ----------------------------------------------------------------------

std::string	Quest::to_json_format(void) const noexcept
{
	std::string	result;

	result = "{";
	result += "\"quest_id\": \"" + name + "\"";
	result += ", ";
	result += "\"description\": \"" + description + "\"";
	result += ", ";
	result += "\"gold_reward\": " + std::to_string(gold_reward);
	result += ", ";
	result += "\"item_reward\": ";
	if (item_reward)
		result += "\"" + item_reward->get_name() + "\"";
	else
		result += "\"None\"";
	result += ", ";
	result += "\"status\": ";
	if (completed)
		result += "\"completed\"";
	else
		result += "\"In progress\"";
	result += "}";
	return (result);
}

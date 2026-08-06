#include "characters/QuestGiver.hpp"

#include "utils/utils.hpp"
#include "characters/Player.hpp"
#include "protocol/responses.hpp"

unsigned int		QuestGiver::available_id = 0;
const std::string	QuestGiver::PREFIX = "quest_giver.";

// Constructors ---------------------------------------------------------------

QuestGiver::QuestGiver(const std::string& name, const std::string& description, Quest& quest, const t_quest_dialogues& dialogues):
	Character(name),
	NPC(NPC::PREFIX + PREFIX + std::to_string(available_id++), name, description),
	quest(quest),
	dialogues(dialogues)
{
}

// Getters and setters --------------------------------------------------------

Quest						QuestGiver::get_quest(void) const noexcept
{
	return (quest);
}

const t_quest_dialogues&	QuestGiver::get_dialogues(void) const noexcept
{
	return (dialogues);
}

// Utils ----------------------------------------------------------------------

const std::string	QuestGiver::on_talk(Player& player) noexcept
{
	const Quest	*quest_found;
	std::string	result;

	(void)player;
	quest_found = nullptr;
	for (const Quest& quest_in_list: player.get_quest_list())
		if (quest.get_name() == quest_in_list.get_name())
			quest_found = &quest_in_list;
	if (quest_found && quest_found->is_completed())
		result = dialogues.finished;
	else if (quest_found && !quest_found->is_completed())
		result = dialogues.already_given;
	else
		result = dialogues.intro;
	return (result);
}

std::string			QuestGiver::request_quest(Player& player) noexcept
{
	const Quest	*quest_found;

	quest_found = nullptr;
	for (const Quest& quest_in_list: player.get_quest_list())
		if (quest.get_name() == quest_in_list.get_name())
			quest_found = &quest_in_list;
	if (quest_found)
		return ("");
	if (!player.obtain_quest(quest))
		return ("");
	return (quest.to_json_format());
}

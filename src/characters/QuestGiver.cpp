#include "characters/QuestGiver.hpp"

#include "utils/utils.hpp"
#include "characters/Player.hpp"

unsigned int		QuestGiver::available_id = 0;
const std::string	QuestGiver::PREFIX = "quest_giver.";

// Constructors ---------------------------------------------------------------

QuestGiver::QuestGiver(const std::string& name, const std::string& description, Quest& quest, const t_quest_dialogues& dialogues, unsigned int gold_reward, Item *item_reward):
	Character(name),
	NPC(NPC::PREFIX + PREFIX + std::to_string(available_id++), name, description),
	quest(quest),
	dialogues(dialogues),
	gold_reward(gold_reward),
	item_reward(item_reward)
{
	// ? REVIEW: Check each dialogue format and gold reward?
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

void	QuestGiver::on_talk(Player& player) noexcept
{
	const Quest	*quest_found;

	quest_found = nullptr;
	for (const Quest& quest_in_list: player.get_quest_list())
	{
		if (quest.get_name() == quest_in_list.get_name())
			quest_found = &quest_in_list;
	}
	if (quest_found && quest_found->is_completed())
		player.send_to_client(dialogues.finished);
	else if (quest_found && !quest_found->is_completed())
		player.send_to_client(dialogues.already_given);
	else
	{
		player.send_to_client(dialogues.intro);
		player.obtain_quest(quest);
	}
}

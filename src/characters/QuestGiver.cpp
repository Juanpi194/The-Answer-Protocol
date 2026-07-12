#include "characters/QuestGiver.hpp"

unsigned int		QuestGiver::available_id = 0;
const std::string	QuestGiver::PREFIX = "quest_giver.";

// Constructors ---------------------------------------------------------------

QuestGiver::QuestGiver(const std::string& name, const std::string& description, Quest& quest):
	Character(name),
	NPC(NPC::PREFIX + PREFIX + std::to_string(available_id++), name, description),
	quest(quest)
{
}

// Getters and setters --------------------------------------------------------

Quest	QuestGiver::get_quest(void) const noexcept
{
	return (quest);
}

// Utils ----------------------------------------------------------------------

void	QuestGiver::on_interact(Player& player)
{
	// TODO: Logic ...
}

#include "characters/QuestGiver.hpp"

// Constructors ---------------------------------------------------------------

QuestGiver::QuestGiver(const std::string& id, const std::string& name, const std::string& description, Quest& quest):
	Character(name),
	NPC(id, name, description),
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

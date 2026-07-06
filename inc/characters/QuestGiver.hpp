#pragma once

#include "characters/NPC.hpp"
#include "quests/Quest.hpp"

class QuestGiver: public NPC
{
	private:
		Quest	quest;
	public:
		// Constructors -------------------------------------------------------

		QuestGiver(const std::string& id, const std::string& name, const std::string& description, Quest& quest);
		QuestGiver(const QuestGiver& quest_giver) = delete;
		~QuestGiver(void) = default;

		// Operators ----------------------------------------------------------

		QuestGiver&	operator=(const QuestGiver& other) = delete;

		// Getters and setters ------------------------------------------------

		/**
		 * @returns	A copy of the quest.
		 */
		Quest	get_quest(void) const noexcept;

		// Utils --------------------------------------------------------------

		void	on_interact(Player& player) override;
};

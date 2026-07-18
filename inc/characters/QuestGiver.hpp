#pragma once

#include "characters/NPC.hpp"
#include "quests/Quest.hpp"

/**
 * @brief	Struct that contains 3 strings, each of them shown
 * 			depending on the quest status.
 * @param	intro	If the the player interacting with the npc
 * 					does not have the quest.
 * @param	already_given	If the quest was already given to the player
 * 							but not completed.
 * @param	finished	If the quest was already given to the player and
 * 						completed.
 */
struct t_quest_dialogues
{
	std::string	intro;
	std::string	already_given;
	std::string	finished;
};

class QuestGiver final: public NPC
{
	private:
		Quest				quest;
		t_quest_dialogues	dialogues;
		unsigned int		gold_reward;
		Item				*item_reward;

		static unsigned int				available_id;	// Defined in QuestGiver.cpp
		static const std::string		PREFIX;	// Defined in QuestGiver.cpp
		static constexpr unsigned int	MIN_DIALOGUE_LENGTH = 25;
		static constexpr unsigned int	MAX_DIALOGUE_LENGTH = 250;
		static constexpr unsigned int	MAX_GOLD_REWARD = 1000;
	public:
		// Constructors -------------------------------------------------------

		QuestGiver(const std::string& name, const std::string& description, Quest& quest, const t_quest_dialogues& dialogues, unsigned int gold_reward, Item *item_reward);

		/**
		 * @note	It is not allowed to make copies because each quest giver should be unique.
		 */
		QuestGiver(const QuestGiver& quest_giver) = delete;
		~QuestGiver(void) = default;

		// Operators ----------------------------------------------------------

		QuestGiver&	operator=(const QuestGiver& other) = delete;

		// Getters and setters ------------------------------------------------

		/**
		 * @returns	A copy of the quest.
		 */
		Quest						get_quest(void) const noexcept;

		const t_quest_dialogues&	get_dialogues(void) const noexcept;

		// Utils --------------------------------------------------------------

		void	on_interact(Player& player) override;
};

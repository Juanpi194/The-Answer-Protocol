#pragma once
#include <string>

class Item;
class Player;

enum class ObjectiveType
{
	DEFEAT_ENEMY,
	COLLECT_ITEM,
	TALK_TO_NPC,
	REACH_AREA,
	EARN_GOLD,
	REACH_LEVEL
};

struct t_quest_objective
{
	ObjectiveType	type;
	std::string		target;
	unsigned int	amount;
};

class Quest
{
	private:
		const std::string	name;
		const std::string	description;
		bool				completed;
		unsigned int		gold_reward;
		Item				*item_reward;
		t_quest_objective	objective;

		static constexpr size_t	MIN_NAME_LENGTH = 8;
		static constexpr size_t	MAX_NAME_LENGTH = 30;
		static constexpr size_t	MIN_DESCRIPTION_LENGTH = 15;
		static constexpr size_t	MAX_DESCRIPTION_LENGTH = 50;
	public:
		// Constructors -------------------------------------------------------

		Quest(const std::string& name, const std::string& description, unsigned int gold_reward, Item *item_reward, const t_quest_objective& objective);
		Quest(const Quest& quest);
		~Quest(void) = default;

		// Operators ----------------------------------------------------------

		Quest&	operator=(const Quest& other) = delete;

		// Getters and setters ------------------------------------------------

		std::string					get_name(void) const noexcept;
		std::string					get_description(void) const noexcept;
		bool						is_completed(void) const noexcept;
		unsigned int				get_gold_reward(void) const noexcept;
		Item						*get_item_reward(void) const noexcept;
		const t_quest_objective&	get_objective(void) const noexcept;

		void		set_completed(bool completed) noexcept;

		// Utils --------------------------------------------------------------

		std::string	to_json_format(void) const noexcept;
};

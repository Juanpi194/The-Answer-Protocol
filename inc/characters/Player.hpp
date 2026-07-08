#pragma once

#include "characters/Fighter.hpp"
#include "quests/Quest.hpp"

class Room;
class Enemy;
class Merchant;
class PlayerConnection;

// Server stuff is managed in PlayerConnection class
class Player final: public Fighter
{
	private:
		// PlayerConnection	*player_connection;
		// ! REVIEW: Check if this is the best way to access the fd.

		/**
		 * @brief	fd of the client that is playing
		 * 			with this Player. If no client is created with
		 * 			this Player, it will be set to -1.
		 * @note	When the client changes its fd,
		 * 			this should change with it.
		 */
		int					client_fd;
		Room				*current_location;
		unsigned int		gold;
		std::list<Enemy*>	beaten_enemies;
		std::list<Quest>	quest_list;
	public:
		// Constructors -------------------------------------------------------

		// ! REVIEW: Is it needed to have a pointer to the player_connection?
		// Player(const std::string& name, PlayerConnection *player_connection);
		Player(const std::string& name);
		Player(const Player& player) = delete;
		~Player(void) = default;

		// Operators ----------------------------------------------------------

		Player&	operator=(const Player& other) = delete;

		// Getters and setters ------------------------------------------------

		// PlayerConnection			*get_player_connection(void) const noexcept;
		Room						*get_current_location(void) const noexcept;
		unsigned int				get_gold(void) const noexcept;
		std::list<Enemy*>&			get_beaten_enemies(void) noexcept;
		const std::list<Enemy*>&	get_beaten_enemies(void) const noexcept;
		std::list<Quest>&			get_quest_list(void) noexcept;
		const std::list<Quest>&		get_quest_list(void) const noexcept;

		// Utils --------------------------------------------------------------

		// Items --

		void			obtain_item(Item *item);
		void			drop_item(Item *item);
		void			buy_item(const Merchant& merchant, Item *item);

		// Location --

		void			move(void);

		// Fight --

		void			choose_action(void) override;
		void			attack(Fighter& target) noexcept override;
		FighterType		get_type(void) const noexcept override;

		// Other --

		void			interact_with(Character& character);
		void			on_interact(Player& player) override;
};

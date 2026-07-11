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
		unsigned int		gold;
		// ? REVIEW: Is this list now needed that every enemy will be a copy?
		// ?		 Maybe make it a list of strings,
		// ?		 or a map with the ammount of each one.
		std::list<Enemy*>	beaten_enemies;
		std::list<Quest>	quest_list;
	public:
		// Constructors -------------------------------------------------------

		// ! REVIEW: Is it needed to have a pointer to the player_connection?
		// Player(const std::string& name, PlayerConnection *player_connection);
		explicit Player(const std::string& name);
		Player(const Player& player) = delete;
		~Player(void) = default;

		// Operators ----------------------------------------------------------

		Player&	operator=(const Player& other) = delete;

		// Getters and setters ------------------------------------------------

		// PlayerConnection			*get_player_connection(void) const noexcept;
		unsigned int				get_gold(void) const noexcept;
		std::list<Enemy*>&			get_beaten_enemies(void) noexcept;
		const std::list<Enemy*>&	get_beaten_enemies(void) const noexcept;
		std::list<Quest>&			get_quest_list(void) noexcept;
		const std::list<Quest>&		get_quest_list(void) const noexcept;

		// Utils --------------------------------------------------------------

		// Items --

		/**
		 * @brief		Adds the specified item to the player's item list
		 * 				and removes it from the current room.
		 * @param		item	The item to be obtained by the player.
		 * @returns		`true` if the item was obtained, `false` otherwise.
		 * @deprecated	Looking for an item requires its name, not the item
		 * 				itself. That's why this method will not be used.
		 * 				Use the one that receives a string instead.
		 */
		bool			obtain_item(Item *item) noexcept __nonnull();
		
		/**
		 * @brief	Tries to find an item with the specified name in the
		 * 			player's room. If it gets find, it will be removed from
		 * 			the room and added to the player's item list.
		 * @param	item_name	The name of the item to find.
		 * @returns	`true` if the item was successfully added to the players
		 * 			item list. `false` otherwise (including not finding
		 * 			the item in the room).
		 * @note	`item_name` can be either a name or an id.
		 */
		bool			obtain_item(const std::string& item_name) noexcept;

		/**
		 * @brief		Adds the specified item to the current room's item list
		 * 				and removes it from the player.
		 * @param		item	The item to be dropped by the player.
		 * @returns		`true` if the item was dropped, `false` otherwise.
		 * @deprecated	Looking for an item requires its name, not the item
		 * 				itself. That's why this method will not be used.
		 * 				Use the one that receives a string instead.
		 */
		bool			drop_item(Item *item) noexcept __nonnull();

		/**
		 * @brief	Tries to find an item with the specified name in the
		 * 			player's item list. If it gets find, it will be reomoved
		 * 			from the player and added to the current room's item list.
		 * @param	item_name	The name of the item to find.
		 * @returns	`true` if the item was successfully added to the current
		 * 			room item list. `false` otherwise (including not finding
		 * 			the item in the room).
		 * @note	`item_name` can be either a name or an id.
		 */
		bool			drop_item(const std::string& item_name) noexcept;

		void			buy_item(const Merchant& merchant, Item *item);

		// Location --

		/**
		 * @brief	Moves the player to a room that exists in the specified
		 * 			direction.
		 * @param	direction	The chosen direction.
		 * @returns	`true` if the player successfully moved to the neighbor,
		 * 			`false` if the player has `current_room` as `nullptr` or
		 * 			if there is no room in that direction.
		 */
		bool			move(Direction direction) noexcept;

		// Fight --

		void			choose_action(void) override;
		void			attack(Fighter& target) noexcept override;
		FighterType		get_type(void) const noexcept override;

		// Other --

		void			interact_with(Character& character);
		void			on_interact(Player& player) override;

		// User --

		// TODO: Finish documentation of the methods below.

		/**
		 * @throws	`std::runtime_error` if input reading fails or EOF.
		 */
		std::string		receive_command(void);

		/**
		 * @brief	Sends a message to the fd of this player. If `client_fd`
		 * 			is `-1`, the message will be sent to `cout`.
		 * @param	msg	The message to send.
		 * @throws
		 */
		void			send_to_client(const std::string& msg) const;
};

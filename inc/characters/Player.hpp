#pragma once

#include "characters/Fighter.hpp"
#include "quests/Quest.hpp"
#include "utils/attributes.hpp"
#include "utils/types.hpp"

class Room;
class Enemy;
class Merchant;
class PlayerConnection;
class Battle;
class Enchantment;

// Server stuff is managed in PlayerConnection class

class Player final: public Fighter
{
	private:
		unsigned int					gold;

		/**
		 * @brief	Register of all enemies that were defeated by
		 * 			the player (their ids). Each player will only
		 * 			be able to defeat each enemy once per game.
		 */
		std::list<std::string>			beaten_enemies_id;
		std::list<Quest>				quest_list;
		std::list<Enchantment*>			enchantment_list;
		std::list<std::string>			outbox;
		Battle							*battle;

		static constexpr unsigned int	STARTING_GOLD = 50;
	public:
		// Constructors -------------------------------------------------------

		explicit Player(const std::string& name);
		Player(const Player& player) = delete;
		~Player(void);

		// Operators ----------------------------------------------------------

		Player&	operator=(const Player& other) = delete;

		// Getters and setters ------------------------------------------------

		unsigned int					get_gold(void) const noexcept;
		const std::list<std::string>&	get_beaten_enemies(void) const noexcept;
		const std::list<Quest>&			get_quest_list(void) const noexcept;
		const std::list<Enchantment*>&	get_enchantment_list(void) const noexcept;
		std::list<std::string>&			get_outbox(void) noexcept;
		Battle							*get_battle(void) const noexcept;

		void	set_battle(Battle *battle) noexcept;

		// Utils --------------------------------------------------------------

		// Items --

		/**
		 * @brief		Adds the specified item (that MUST be in the player's
		 * 				current room) to the player's item list
		 * 				and removes it from the current room.
		 * @param		item	The item to be obtained by the player.
		 * @returns		`true` if the item was obtained, `false` otherwise.
		 * @deprecated	Looking for an item requires its name, not the item
		 * 				itself. That's why this method will not be used.
		 * 				Use the one that receives a string instead.
		 */
		bool			obtain_item(Item *item) noexcept TAP_NONNULL TAP_DEPRECATED;

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
		bool			drop_item(Item *item) noexcept TAP_NONNULL TAP_DEPRECATED;

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

		// Enchantments --

		/**
		 * @brief	Simply adds an enchantment to the player's enchantment list,
		 * 			without any checking.
		 * @param	enchantment	The enchantment to be added.
		 */
		void			add_enchantment(Enchantment *enchantment) noexcept TAP_NONNULL;

		/**
		 * @brief	Removes an enchantment from the player's enchantment list,
		 * 			removing it from the list and deleting it.
		 * @param	enchantment	The enchantment to delete.
		 * @throws	`std::invalid_argument` if the enchantment is not in the player's
		 * 			enchantment list.
		 */
		void			consume_enchantment(Enchantment& enchantment);

		/**
		 * @brief	Tries to find an enchantment in the player's enchantment
		 * 			list that matches the specified name.
		 * @param	enchantment_name	The name of the enchantment to search.
		 * @returns	The enchantment found with the name, `nullptr` if no
		 * 			enchantment was found with the specified name.
		 */
		Enchantment		*find_enchantment_by_name(const std::string& enchantment_name) const TAP_UNUSED_RESULT;

		// Quests --

		/**
		 * @brief	The provided quest is added to the player's quest list.
		 * @param	quest	The quest to add.
		 * @returns	`true` if the player successfully received the quest.
		 * 			`false` otherwise.
		 * @note	If the quest is already in the list, it will not be added.
		 */
		bool			obtain_quest(Quest& quest) noexcept TAP_COLD;

		// Gold --

		/**
		 * @brief	Tries to consume a specified ammount of gold. If the player
		 * 			does not have enough gold, it won't be consumed.
		 * @param	quantity	The ammount of gold to consume.
		 * @returns	`true` if the player had enough gold to spend and it was
		 * 			successfully consumed, `false` if the player does not have
		 * 			enough gold.
		 * @note	The value returned by this method MUST be used.
		 */
		bool			spend_gold(unsigned int quantity) noexcept TAP_UNUSED_RESULT;

		/**
		 * @brief	Unlike the `spend_gold` method, this one will always
		 * 			consume the specified ammount of gold, setting it to 0
		 * 			if the player has less gold than the specified one.
		 * @param	quantity	The ammount of gold to consume.
		 */
		void			lose_gold(unsigned int quantity) noexcept;

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

		/**
		 * @brief	Changes the player's current room to the spawn room,
		 * 			and sets his current hp to half.
		 * @param	destination	The room to set as the current room. It cannot
		 * 			be `nullptr`.
		 * @throws	`std::runtime_error` if `current_room` is `nullptr`.
		 * 			`std::invalid_argument` if `remove_player` or `add_player`
		 * 			`Room` methods fail.
		 */
		void			respawn(Room *destination) TAP_NONNULL;

		// Fight --

		bool			is_enemy_beaten(Enemy *enemy) noexcept TAP_NONNULL TAP_UNUSED_RESULT;
		void			choose_action(void) override;
		void			attack(Fighter& target) noexcept override;
		FighterType		get_type(void) const noexcept override;

		// Interactions --

		void			talk_with(Character& character);
		void			on_talk(Player& player) noexcept override;

		// User --

		// TODO: Finish documentation of the methods below.

		/**
		 * @brief	Sends a message to the fd of this player. If `client_fd`
		 * 			is `-1`, the message will be sent to `cout`.
		 * @param	msg	The message to send.
		 */
		void			send_to_client(const std::string& msg) noexcept;
};

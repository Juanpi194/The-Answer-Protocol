#pragma once
#include <string>
#include <list>
#include <map>

#include "utils/types.hpp"

class Player;
class NPC;
class Item;
class Chest;

class Room
{
	private:
		const std::string			id;
		const std::string			name;
		const std::string			description;
		NPC							*npc;
		Chest						*chest;
		std::list<Item*>			items;
		std::list<Player*>			player_list;
		std::map<Direction, Room*>	adyacent_rooms;

		static const std::string	PREFIX;	// Defined in Room.cpp
		static constexpr bool		TITLE_NAME = true;
		static constexpr size_t		MIN_NAME_LENGTH = 3;
		static constexpr size_t		MAX_NAME_LENGTH = 18;
		static constexpr size_t		MIN_DESCRIPTION_LENGTH = 10;
		static constexpr size_t		MAX_DESCRIPTION_LENGTH = 60;

		/**
		 * @brief	Checks that all arguments for the room initialization are valid.
		 * @returns	`true` if passes the validation process. `false` otherwise.
		 * @note	This method should be used ONLY in the constructor.
		 */
		static bool	validate_arguments(const std::string& id, const std::string& name, const std::string& description);
	public:
		// Constructors -------------------------------------------------------

		/**
		 * @throws	`std::invalid_argument` if validation failed or
		 * 			if the item list contains any `nullptr`.
		 */
		Room(const std::string& id, const std::string& name, const std::string& description, NPC *npc, Chest *chest, std::list<Item*>& items);
		Room(const Room& zone) = delete;
		virtual ~Room(void);

		// Operators ----------------------------------------------------------

		Room&	operator=(const Room& other) = delete;

		// Getters and setters ------------------------------------------------

		std::string							get_id(void) const noexcept;
		std::string							get_name(void) const noexcept;
		std::string							get_description(void) const noexcept;
		NPC									*get_NPC(void) const noexcept;
		Chest								*get_chest(void) const noexcept;
		std::list<Item*>&					get_items(void) noexcept;
		const std::list<Item*>&				get_items(void) const noexcept;
		std::list<Player*>&					get_player_list(void) noexcept;
		const std::list<Player*>& 			get_player_list(void) const noexcept;
		std::map<Direction, Room*>&			get_adyacent_rooms(void) noexcept;
		const std::map<Direction, Room*>&	get_adyacent_rooms(void) const noexcept;

		void	set_adyacent_room(Direction direction, Room *room);

		// Utils --------------------------------------------------------------

		/**
		 * @brief	Add an item to the list of items of this room.
		 * @param	item	The item to add to the list.
		 * @throws	`std::invalid_argument` if `item` is `nullptr` or if
		 * 			`item` is already in the list.
		 */
		void	add_item(Item *item) __nonnull();

		/**
		 * @brief	Adds a player to the list of players of this room.
		 * @param	player	The player to be added.
		 * @throws	`std::invalid_argument` if `player` is `nullptr` or if
		 * 			`player` is already in the list.
		 */
		void	add_player(Player *player) __nonnull();

		/**
		 * @brief	Removes an item from the list of items of this room.
		 * @param	item	The item to be removed.
		 * @throws	`std::invalid_argument` if `item` is `nullptr` or if
		 * 			`item` is not in the list.
		 */
		void	remove_item(Item *item) __nonnull();

		/**
		 * @brief	Removes a player from the list of players of this room.
		 * 			This means that after this method, player's `current_room`
		 * 			will be `nullptr`.
		 * @param	player	The player to be removed.
		 * @throws	`std::invalid_argument` if `player` is `nullptr` or if
		 * 			`player` is not in the list.
		 */
		void	remove_player(Player *player) __nonnull();

		// ? REVIEW: Should we do these methods?

		/**
		 * @brief	Removed the specified item if its in the list, and
		 * 			it also returns it.
		 * @param	item	The item to remove. Assign as `nullptr` if you
		 * 					want the last item to be removed.
		 * @returns	`item` if found, or the last item of the list if
		 * 			`item` is `nullptr`.
		 * @throws	`std::invalid_argument` if `item` is not `nullptr` and its
		 * 			not in the item list.
		 */
		Item	*pop_item(Item *item = nullptr);

		// virtual void	on_enter(void) noexcept = 0;

		void			clear(void);

		void			connect_room();

		/**
		 * @brief	Generates the room information in json format.
		 * @returns	The json format as `string`.
		 */
		const std::string	look(void) const noexcept;
};

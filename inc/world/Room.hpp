#pragma once
#include <string>
#include <list>
#include <map>

#include "utils/types.hpp"

class Player;
class NPC;
class Item;

class Room
{
	private:
		const std::string			id;
		const std::string			name;
		const std::string			description;
		NPC							*npc;
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

		Room(const std::string& id, const std::string& name, const std::string& description, NPC *npc, std::list<Item*>& items);
		Room(const Room& zone) = delete;
		virtual ~Room(void);

		// Operators ----------------------------------------------------------

		Room&	operator=(const Room& other) = delete;

		// Getters and setters ------------------------------------------------

		std::string							get_id(void) const noexcept;
		std::string							get_name(void) const noexcept;
		std::string							get_description(void) const noexcept;
		NPC									*get_NPC(void) const noexcept;
		std::list<Item*>&					get_items(void) noexcept;
		const std::list<Item*>&				get_items(void) const noexcept;
		std::list<Player*>&					get_player_list(void) noexcept;
		const std::list<Player*>& 			get_player_list(void) const noexcept;
		std::map<Direction, Room*>&			get_adyacent_rooms(void) noexcept;
		const std::map<Direction, Room*>&	get_adyacent_rooms(void) const noexcept;

		void	set_adyacent_room(Direction direction, Room *room);

		// Utils --------------------------------------------------------------

		virtual void	on_enter(void) noexcept = 0;
		void			clear(void);
		// void			connect_room()
};

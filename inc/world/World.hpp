#pragma once

#include "world/Room.hpp"
#include "utils/attributes.hpp"

class World
{
	private:
		const std::string	name;
		std::list<Room*>	rooms;
		Room				*spawn_room;

		static constexpr size_t	MIN_NAME_LENGTH = 4;
		static constexpr size_t	MAX_NAME_LENGTH = 40;
		static constexpr bool	TITLE_NAME = true;
		static constexpr size_t	MAX_ROOMS = 15;

		/**
		 * @brief	Checks that the provided name for the world
		 * 			follows the specified rules.
		 * @returns	`true` if validation is passed. `false` otherwise.
		 * @note	This method should ONLY be used when initializing the world.
		 */
		static bool	validate_name(const std::string& name);

		/**
		 * @brief	Checks that the provided json with all the configuration
		 * 			is a valid file and that the configuration has good
		 * 			format.
		 * @returns	`true` if the json format is correct, `false` otherwise.
		 * @note	This method should ONLY be used when initializing the world.
		 */
		static bool	validate_json(const std::string& json_path);

		/**
		 * @brief	Checks if a room exists in the world.
		 * @param	room	The room to evaluate.
		 * @returns	`true` if `room` exists in the world. `false` if
		 * 			`room` is not in the world list of rooms.
		 * @note	`room` must not be `nullptr`.
		 */
		bool	room_in_world(Room *room) TAP_NONNULL;
	public:
		// Constructors -------------------------------------------------------

		/**
		 * @brief	Default world.
		 */
		explicit World(const std::string& name);

		/**
		 * @brief	World with data in the json.
		 */
		World(const std::string& name, const std::string& json_path);
		World(const World& world) = delete;

		/**
		 * @brief	All characters, items, ... must be freed from here.
		 * 			They will only exist in this instance.
		 * 
		 */
		~World(void);

		// Operators ----------------------------------------------------------

		World&	operator=(const World& other) = delete;

		// Getters and setters ------------------------------------------------

		std::string				get_name(void) const noexcept;
		// std::list<Room*>&		get_rooms(void) noexcept;
		const std::list<Room*>&	get_rooms(void) const noexcept;
		Room					*get_spawn_room(void) const noexcept;

		void	set_spawn_room(Room *room) noexcept TAP_NONNULL;

		// Utils --------------------------------------------------------------

		// ? REVIEW: Does this method make sense?
		// void	reset_world(void);

		/**
		 * @brief	Adds a new room to the world, connecting it to an already
		 * 			existing room in the world in a specified direction from
		 * 			that room. The room must not exist in the world.
		 * @param	new_room	Pointer to the new room to add.
		 * @param	connected_to	Pointer to the room that `new_room` will be
		 * 							connected to.
		 * @param	direction	The direction that connects `connected_to` to
		 * 						`new_room`.
		 * @returns	`true` if the room was successfully added.
		 * 			`false` otherwise.
		 * @throws	`std::invalid_argument` if any of the pointers are
		 * 			`nullptr`, if `new_room` and `connected_to` are the same,
		 * 			if `new_room` is already in the world,
		 * 			or if the room it is `connected to` has another room
		 * 			in the specified direction.
		 * @example	direction = EAST -> `connected_to` has `new_room` at east.
		 */
		bool	add_room(Room *new_room, Room *connected_to, Direction direction) TAP_NONNULL TAP_UNUSED_RESULT;
};

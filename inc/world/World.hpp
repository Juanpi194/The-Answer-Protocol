#pragma once

#include "world/Room.hpp"

class World
{
	private:
		const std::string	name;
		std::list<Room*>	rooms;

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
	public:
		// Constructors -------------------------------------------------------

		/**
		 * @brief	Default world.
		 */
		World(const std::string& name);

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
		std::list<Room*>&		get_rooms(void) noexcept;
		const std::list<Room*>&	get_rooms(void) const noexcept;

		// Utils --------------------------------------------------------------

		void	reset_world(void);
};

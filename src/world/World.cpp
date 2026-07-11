#include "world/World.hpp"

#include <stdexcept>

#include "utils/utils.hpp"

/**
 * @brief	Obtains the opposite direction of the specified direction.
 * @note	A static function that obtains the opposite direction
 * 			of a specified direction would consume less resources, but this
 * 			is easier to read.
 */
static const std::map<Direction, Direction>	OPPOSITE_DIRECTIONS = {
	{Direction::NORTH, Direction::SOUTH},
	{Direction::SOUTH, Direction::NORTH},
	{Direction::EAST, Direction::WEST},
	{Direction::WEST, Direction::EAST}
};

bool	World::validate_name(const std::string& name)
{
	std::string	temp = name;

	trim_str(temp);
	if (temp.empty())
		return (log("World name cannot be empty.", LogLevel::ERROR), false);
	if (temp.size() < MIN_NAME_LENGTH)
		return (log("World name is too short (MIN CHARACTERS: " + std::to_string(MIN_NAME_LENGTH) + ").", LogLevel::ERROR), false);
	if (temp.size() > MAX_NAME_LENGTH)
		return (log("World name is too long (MAX CHARACTERS: " + std::to_string(MAX_NAME_LENGTH) + ").", LogLevel::ERROR), false);
	if (TITLE_NAME && !is_title(temp))
		return (log("World name must be have title format.", LogLevel::ERROR), false);
	return (true);
}

bool	World::validate_json(const std::string& json_path)
{
	if (!ends_with(json_path, ".json") && !ends_with(json_path, ".jsonc"))
		return (log("Provided json file path sufix must be '.json' or '.jsonc'.", LogLevel::ERROR), false);
	// TODO: Implement validation logic
	return (true);
}

bool	World::room_in_world(Room *room)
{
	bool	found;

	found = false;
	if (!room)
		return (false);
	for (Room *room_in_list: rooms)
	{
		if (room_in_list == room)
			found = true;
	}
	return (found);
}

// Constructors ---------------------------------------------------------------

World::World(const std::string& name):
	name(name)
{
	if (!validate_name(name))
		throw std::invalid_argument("World name validation failed.");
	// TODO: Create default world
}

World::World(const std::string& name, const std::string& json_path)
{
	if (!validate_name(name) || !validate_json(json_path))
		throw std::invalid_argument("World validation failed.");
	// TODO: Create world with json config
}

World::~World(void)
{
	// TODO: Delete all npcs, items, ...
	for (Room *room: rooms)
		delete (room);
}

// Getters and setters --------------------------------------------------------

std::string				World::get_name(void) const noexcept
{
	return (name);
}

std::list<Room*>&		World::get_rooms(void) noexcept
{
	return (rooms);
}

const std::list<Room*>&	World::get_rooms(void) const noexcept
{
	return (rooms);
}

// Utils ----------------------------------------------------------------------

void	World::add_room(Room *new_room, Room *connected_to, Direction direction)
{
	if (!new_room || !connected_to)
		throw std::invalid_argument("Tried to add a room with not valid parameters.");
	if (new_room == connected_to)
		throw std::invalid_argument("Tried to add a new room connected to itself.");
	if (room_in_world(new_room))
		throw std::invalid_argument("New room to add already exists in the world.");
	if (!room_in_world(connected_to))
		throw std::invalid_argument("Room that connects with the new room does not exist in the world.");
	if (connected_to->get_adyacent_rooms().count(direction) > 0)
		throw std::invalid_argument("The room that is being connected with the new room already has a room at the specified direction.");

	// Validation passed
	rooms.push_back(new_room);	
	connected_to->get_adyacent_rooms()[direction] = new_room;
	new_room->get_adyacent_rooms()[OPPOSITE_DIRECTIONS.at(direction)] = connected_to;
	log("New room '" + new_room->get_name() + "' was connected to '" + connected_to->get_name() + "'.", LogLevel::DEBUG);
}

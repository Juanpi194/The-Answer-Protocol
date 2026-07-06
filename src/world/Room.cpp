#include "world/Room.hpp"

#include <iostream>

#include "utils/utils.hpp"

const std::string	Room::PREFIX = "room.";

bool	Room::validate_arguments(const std::string& id, const std::string& name, const std::string& description)
{
	std::string	room_id;
	std::string	room_name;
	std::string	room_description;

	// ID
	room_id = id;
	if (room_id.empty())
		return (log("Room id cannot be empty.", LogLevel::ERROR), false);
	if (!starts_with(room_id, PREFIX))
		return (log("Room id must start with '" + PREFIX + "'.", LogLevel::ERROR), false);
	room_id = id.substr(PREFIX.size());
	trim_str(room_id);
	if (room_id.empty())
		return (log("Room id name cannot be empty (After prefix).", LogLevel::ERROR), false);
	// TODO: Check if there are spaces in the ID (there shouldn't be)
	if (room_id.size() > MAX_NAME_LENGTH)
		return (log("Room id name is too long (MAX CHARACTERS: " + std::to_string(MAX_NAME_LENGTH) + ").", LogLevel::ERROR), false);
	if (room_id.size() < MIN_NAME_LENGTH)
		return (log("Room id name is too short (MIN CHARACTERS: " + std::to_string(MIN_NAME_LENGTH) + ").", LogLevel::ERROR), false);

	// NAME
	room_name = name;
	trim_str(room_name);
	if (room_name.empty())
		return (log("Room name cannot be empty.", LogLevel::ERROR), false);
	if (TITLE_NAME && !is_title(room_name))
		return (log("Room name must be a title.", LogLevel::ERROR), false);
	if (room_name.size() > MAX_NAME_LENGTH)
		return (log("Room name is too long (MAX CHARACTERS: " + std::to_string(MAX_NAME_LENGTH) + ").", LogLevel::ERROR), false);
	if (room_name.size() < MIN_NAME_LENGTH)
		return (log("Room name is too short (MIN CHARACTERS: " + std::to_string(MIN_NAME_LENGTH) + ").", LogLevel::ERROR), false);

	// DESCRIPTION
	room_description = description;
	trim_str(room_description);
	if (room_description.empty())
		return (log("Room description cannot be empty.", LogLevel::ERROR), false);
	if (room_description.size() > MAX_DESCRIPTION_LENGTH)
		return (log("Room description is too long (MAX CHARACTERS: " + std::to_string(MAX_DESCRIPTION_LENGTH) + ").", LogLevel::ERROR), false);
	if (room_description.size() < MIN_DESCRIPTION_LENGTH)
		return (log("Room description is too short (MIN CHARACTERS: " + std::to_string(MIN_DESCRIPTION_LENGTH) + ").", LogLevel::ERROR), false);

	// VALIDATION PASSED
	return (true);
}

// Constructors ---------------------------------------------------------------

Room::Room(const std::string& id, const std::string& name, const std::string& description, NPC *npc, Item *item):
	id(id),
	name(name),
	description(description),
	npc(npc),
	item(item)
{
	if (!validate_arguments(id, name, description))
		throw std::invalid_argument("Provided room arguments are not valid. Room initialization failed.");
	if (!npc)
		log("No npc established in room with id '" + id + "'.", LogLevel::INFO);
	if (!item)
		log("No item established in room with id '" + id + "'.", LogLevel::INFO);
}

Room::~Room()
{
	// TODO: Clear players, items, ...
	this->clear();
}

// Getters and setters --------------------------------------------------------

std::string							Room::get_id(void) const noexcept
{
	return (id);
}

std::string							Room::get_name(void) const noexcept
{
	return (name);
}

std::string							Room::get_description(void) const noexcept
{
	return (description);
}

NPC									*Room::get_NPC(void) const noexcept
{
	return (npc);
}

Item								*Room::get_item(void) const noexcept
{
	return (item);
}

std::list<Player*>&					Room::get_player_list(void) noexcept
{
	return (player_list);
}

const std::list<Player*>& 			Room::get_player_list(void) const noexcept
{
	return (player_list);
}

std::map<Direction, Room*>&			Room::get_adyacent_rooms(void) noexcept
{
	return (adyacent_rooms);
}

const std::map<Direction, Room*>&	Room::get_adyacent_rooms(void) const noexcept
{
	return (adyacent_rooms);
}

void	Room::set_item(Item *item)
{
	this->item = item;
	if (!item)
		log("Item 'nullptr' was assigned to room " + name + ".", LogLevel::INFO);
}

void	Room::set_adyacent_room(Direction direction, Room *room)
{
	// TODO: Check if a room exists in that direction, decide ...
}

// Utils ----------------------------------------------------------------------

void	Room::clear(void)
{
	// TODO: Kick players, items, ...
}

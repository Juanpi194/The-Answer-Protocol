#include "world/Room.hpp"

#include <stdexcept>

#include "utils/utils.hpp"
#include "utils/types.hpp"
#include "characters/NPC.hpp"
#include "characters/Player.hpp"
#include "items/Chest.hpp"

const std::string Room::PREFIX = "room.";

bool Room::validate_arguments(const std::string& id, const std::string& name, const std::string& description)
{
	std::string	room_id;
	std::string room_name;
	std::string room_description;

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
	if (has_space(room_id))
		return (log("Room id cannot have spaces.", LogLevel::ERROR), false);
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

Room::Room(const std::string& id, const std::string& name, const std::string& description, NPC *npc, Chest *chest, std::list<Item *>& items):
	id(id),
	name(name),
	description(description),
	npc(npc),
	chest(chest),
	items(items)
{
	if (!validate_arguments(id, name, description))
		throw std::invalid_argument("Provided room arguments are not valid. Room initialization failed.");
	if (!npc)
		log("No npc established in room with id '" + id + "'.", LogLevel::INFO);
	if (!chest)
		log("No chest established in room with id '" + id + "'.", LogLevel::INFO);
	if (items.size() == 0)
		log("No items established in room with id '" + id + "'.", LogLevel::INFO);
	else
	{
		for (Item *item: items)
		{
			if (!item)
				throw std::invalid_argument("Item list cannot contain a nullptr.");
		}
	}
}

Room::~Room()
{
	// NPC
	if (npc)
		delete (npc);

	// Chest
	if (chest)
		delete (chest);

	// Items
	for (Item *item: items)
		delete (item);

	// Players
	for (Player *player: player_list)
		player->set_current_room(nullptr);
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

Chest								*Room::get_chest(void) const noexcept
{
	return (chest);
}

// std::list<Item *>&					Room::get_items(void) noexcept
// {
// 	return (items);
// }

const std::list<Item*>&				Room::get_items(void) const noexcept
{
	return (items);
}

// std::list<Player*>&					Room::get_player_list(void) noexcept
// {
// 	return (player_list);
// }

const std::list<Player*>&			Room::get_player_list(void) const noexcept
{
	return (player_list);
}

// std::map<Direction, Room*>&			Room::get_adjacent_rooms(void) noexcept
// {
// 	return (adjacent_rooms);
// }

const std::map<Direction, Room*>&	Room::get_adjacent_rooms(void) const noexcept
{
	return (adjacent_rooms);
}

void	Room::set_adjacent_room(Direction direction, Room *room)
{
	// ? REVIEW: Is this method really needed? There is a method in World that connects two rooms.
	// ?		 It can be used as a short and dangerous way to connect rooms. Make docstring if so.
	if (!room)
	{
		log("Tried to connect a nullptr room to '" + name + "'.", LogLevel::WARNING);
		return ;
	}
	this->adjacent_rooms[direction] = room;
}

// Utils ----------------------------------------------------------------------

void				Room::add_item(Item *item)
{
	if (!item)
		throw std::invalid_argument("Cannot add a nullptr to the item list.");
	for (Item *item_in_list: items)
	{
		if (item_in_list == item)
			throw std::invalid_argument("Cannot add the same item twice in the same room.");
	}
	items.push_back(item);
}

void				Room::add_player(Player *player)
{
	if (!player)
		throw std::invalid_argument("Cannot add a nullptr to the player list.");
	for (Player *player_in_list: player_list)
	{
		if (player_in_list == player)
			throw std::invalid_argument("Cannot add the same player twice in the same room.");
	}
	player_list.push_back(player);
	player->set_current_room(this);
}

void				Room::remove_item(Item *item)
{
	bool	found;

	found = false;
	if (!item)
		throw std::invalid_argument("Cannot remove a nullptr from the item list.");
	for (Item *item_in_list: items)
	{
		if (item_in_list == item)
			found = true;
	}
	if (!found)
		throw std::invalid_argument("Item to removed must be in the room.");
	items.remove(item);
}

void				Room::remove_player(Player *player)
{
	bool	found;

	found = false;
	if (!player)
		throw std::invalid_argument("Cannot remove a nullptr from the player list.");
	for (Player *player_in_list: player_list)
	{
		if (player_in_list == player)
			found = true;
	}
	if (!found)
		throw std::invalid_argument("Player to remove must be in the room.");
	player_list.remove(player);
	player->set_current_room(nullptr);
}

void				Room::clear(void)
{
	// TODO: Kick players (setting their room to nullptr), free items, ...
}

const std::string	Room::look(void) const noexcept
{
	// TODO: Add chest and extras.
	// ? REVIEW: Full review of everything.
	std::string	result;
	bool		first;

	result += "{ \"room\": { ";
	result += "\"id\": \"" + id + "\", ";
	result += "\"name\": \"" + name + "\", ";
	result += "\"description\": \"" + description + "\", ";
	result += "\"exits\": ";
	if (adjacent_rooms.size() == 0)
		result += "\"None\"";
	else
	{
		first = true;
		result += "{";
		for (std::pair<Direction, Room*> direction_and_room: adjacent_rooms)
		{
			// ? REVIEW: Leave it in uppercase, or make a to_lower function for the direction.
			if (!first)
				result += ", ";
			result += "\"" + direction_to_string(direction_and_room.first) + "\": ";
			result += "\"" + direction_and_room.second->get_id() + "\"";
			first = false;
		}
		result += "}";
	}
	result += " }, ";
	result += "\"players\": ";
	if (player_list.size() == 0)
		result += "\"None\"";
	else
	{
		first = true;
		result += "[";
		for (Player *player: player_list)
		{
			if (!first)
				result += ", ";
			result += "\"" + player->get_name() + "\"";
			first = false;
		}
		result += "]";
	}
	result += ", ";
	result += "\"items\": ";
	if (items.size() == 0)
		result += "\"None\"";
	else
	{
		first = true;
		result += "[";
		for (Item *item: items)
		{
			if (!first)
				result += ", ";
			result += "\"" + item->get_id() + "\"";
			first = false;
		}
		result += "]";
	}
	result += ", ";
	result += "\"npc\": ";
	if (!npc)
		result += "\"None\"";
	else
		result += "\"" + npc->get_id() + "\"";
	result += " }";
	return (result);
}

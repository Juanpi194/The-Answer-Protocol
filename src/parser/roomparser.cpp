#include "parser/roomparser.hpp"
#include <unordered_map>
#include "utils/utils.hpp"
#include "world/Room.hpp"


// Crea un nombre más corto (RoomMap) para un mapa que relaciona el id de una habitación con su objeto Room.
typedef std::unordered_map<std::string, Room*> RoomMap;

static Direction parse_direction(const std::string& direction)
{
	if (direction == "north")
		return (Direction::NORTH);
	if (direction == "east")
		return (Direction::EAST);
	if (direction == "south")
		return (Direction::SOUTH);
	if (direction == "west")
		return (Direction::WEST);

	log("Unknown direction '" + direction + "'.", LogLevel::WARNING);
	return (Direction::NORTH);
}

static Room *create_room(const nlohmann::json& room_json)
{
	const std::string id = room_json["id"];
	const std::string name = room_json["name"];
	const std::string description = room_json["description"];

	return (new Room(id, name, description, nullptr, nullptr));
}

static void connect_exits(const nlohmann::json& room_json,
						  RoomMap& all_rooms)
{
	const std::string id = room_json["id"];
	Room *current_room = all_rooms[id];

	for (nlohmann::json::const_iterator it = room_json["exits"].begin();
			it != room_json["exits"].end(); ++it)
	{
		const std::string direction = it.key();
		const std::string target = it.value();

		if (all_rooms.find(target) == all_rooms.end())
		{
			log("Room '" + id + "' has an exit to unknown room '" + target + "'.",
				LogLevel::WARNING);
			continue;
		}

		current_room->set_adyacent_room(
			parse_direction(direction),
			all_rooms[target]);
	}
}

std::list<Room*> RoomParser::parse(const nlohmann::json& rooms_json)
{
	std::list<Room*> rooms;
	RoomMap all_rooms;

	if (!rooms_json.is_array())
		return (rooms);

	// First pass: create every room
	for (nlohmann::json::const_iterator it = rooms_json.begin();
		it != rooms_json.end(); ++it)
	{
		const nlohmann::json& room_json = *it;

		Room *room = create_room(room_json);

		rooms.push_back(room);
		all_rooms[room_json["id"]] = room;
	}

	// Second pass: connect every exit
	for (nlohmann::json::const_iterator it = rooms_json.begin();
		it != rooms_json.end(); ++it)
	{
		const nlohmann::json& room_json = *it;
		connect_exits(room_json, all_rooms);
	}

	return (rooms);
}
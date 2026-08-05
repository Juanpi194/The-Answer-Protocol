#include "parser/roomparser.hpp"
#include <stdexcept>
#include <unordered_map>
#include "characters/Enemy.hpp"
#include "factories/EnemyFactory.hpp"
#include "parser/npcparser.hpp"
#include "utils/types.hpp"
#include "utils/utils.hpp"
#include "world/Room.hpp"


// Crea un nombre más corto (RoomMap) para un mapa que relaciona el id de una habitación con su objeto Room.
typedef std::unordered_map<std::string, Room*> RoomMap;

static NPC *create_enemy(const nlohmann::json& room_json)
{
	const std::string id = room_json["id"];

	if (!room_json.contains("enemy"))
		return (nullptr);

	const std::string enemy_name = room_json["enemy"];

	try
	{
		return (EnemyFactory::create_from_name(enemy_name));
	}
	catch (const std::exception& e)
	{
		log("Room '" + id + "' has an invalid enemy: " + e.what(),
			LogLevel::WARNING);
	}
	return (nullptr);
}

static NPC *create_room_npc(const std::string& id,
                            const nlohmann::json& room_json,
                            const nlohmann::json& npcs_json)
{
	const std::string key = room_json["npc"];

	if (!npcs_json.contains(key))
	{
		log("Room '" + id + "' references unknown npc '" + key + "'.",
			LogLevel::WARNING);
		return (nullptr);
	}

	try
	{
		return (NPCParser::build_npc("npc." + key, npcs_json[key]));
	}
	catch (const std::exception& e)
	{
		log("Room '" + id + "' has an invalid npc '" + key + "': "
			+ e.what(), LogLevel::WARNING);
		return (nullptr);
	}
}

// Devuelve el NPC que ocupa la sala: enemigo, npc pacifico o nullptr.
// Como Room tiene un unico slot, si vienen los dos campos, uno gana.
static NPC *create_occupant(const nlohmann::json& room_json,
                            const nlohmann::json& npcs_json)
{
	const std::string id = room_json["id"];
	const bool has_enemy = room_json.contains("enemy");
	const bool has_npc   = room_json.contains("npc");

	if (has_enemy && has_npc)
		log("Room '" + id + "' has both 'enemy' and 'npc'; using 'enemy'.",
			LogLevel::WARNING);

	if (has_enemy)
		return (create_enemy(room_json));

	if (has_npc)
		return (create_room_npc(id, room_json, npcs_json));

	return (nullptr);
}

static Room *create_room(const nlohmann::json& room_json, const nlohmann::json& npcs_json)
{
	const std::string id = room_json["id"];
	const std::string name = room_json["name"];
	const std::string description = room_json["description"];
	std::list<Item*> items;

	NPC	*npc = create_occupant(room_json, npcs_json);
	return (new Room(id, name, description, npc, false, items));
}

static Room *build_room(const nlohmann::json& room_json, const nlohmann::json& npcs_json)
{
	const std::string id = room_json.value("id", "unknown");

	try
	{
		return (create_room(room_json, npcs_json));
	}
	catch (const std::exception& e)
	{
		log(
			"Could not create room '"
			+ id + "': "
			+ e.what(),
			LogLevel::WARNING);
	}
	return (nullptr);
}

static void connect_exits(const nlohmann::json& room_json,
						  RoomMap& all_rooms)
{
	const std::string id = room_json["id"];
	RoomMap::const_iterator room_it = all_rooms.find(id);

	if (room_it == all_rooms.end())
		return;

	Room *current_room = room_it->second;

	for (nlohmann::json::const_iterator it = room_json["exits"].begin();
			it != room_json["exits"].end(); ++it)
	{
		const std::string direction = it.key();
		const std::string target = it.value();
		Direction dir = string_to_direction(direction);

		if (dir == Direction::INVALID)
		{
			log("Room '" + id + "' has an exit with unknown direction '" + direction + "'.",
				LogLevel::WARNING);
			continue;
		}

		if (all_rooms.find(target) == all_rooms.end())
		{
			log("Room '" + id + "' has an exit to unknown room '" + target + "'.",
				LogLevel::WARNING);
			continue;
		}

		current_room->set_adjacent_room(dir, all_rooms[target]);
	}
}

std::list<Room*> RoomParser::parse(const nlohmann::json& rooms_json, const nlohmann::json& npcs_json)
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

		Room *room = build_room(room_json, npcs_json);

		if (room == nullptr)
			continue;

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
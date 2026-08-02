#include "parser/roomparser.hpp"
#include <unordered_map>
#include "utils/types.hpp"
#include "utils/utils.hpp"
#include "world/Room.hpp"
#include "items/Item.hpp"
#include "characters/NPC.hpp"

// Crea un nombre más corto (RoomMap) para un mapa que relaciona el id de una habitación con su objeto Room.
typedef std::unordered_map<std::string, Room*> RoomMap;

// MODIFIED: ahora lee "items" (array de claves) y "npc" (clave suelta) del
// JSON de la sala, y mueve los Item*/NPC* correspondientes desde los mapas
// globales -- antes siempre se creaba con item_list vacía y npc=nullptr,
// así que ninguna sala terminaba con nada dentro.
static Room *create_room(const nlohmann::json& room_json,
						  std::map<std::string, Item*>& items,
						  std::map<std::string, NPC*>& npcs)
{
	const std::string id = room_json["id"];
	const std::string name = room_json["name"];
	const std::string description = room_json["description"];
	std::list<Item*> room_items;
	NPC *room_npc = nullptr;

	if (room_json.contains("items") && room_json["items"].is_array())
	{
		for (const auto& item_key_json : room_json["items"])
		{
			const std::string item_key = item_key_json.get<std::string>();
			auto found = items.find(item_key);

			if (found == items.end())
			{
				log("Room '" + id + "' references unknown item '" + item_key + "'.",
					LogLevel::WARNING);
				continue;
			}
			room_items.push_back(found->second);
			items.erase(found); // consumido -- ahora es de esta sala
		}
	}

	if (room_json.contains("npc") && room_json["npc"].is_string())
	{
		const std::string npc_key = room_json["npc"].get<std::string>();
		auto found = npcs.find(npc_key);

		if (found == npcs.end())
			log("Room '" + id + "' references unknown npc '" + npc_key + "'.",
				LogLevel::WARNING);
		else
		{
			room_npc = found->second;
			npcs.erase(found); // consumido -- ahora es de esta sala
		}
	}

	return (new Room(id, name, description, room_npc, false, room_items));
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

std::list<Room*> RoomParser::parse(const nlohmann::json& rooms_json,
									std::map<std::string, Item*>& items,
									std::map<std::string, NPC*>& npcs)
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

		Room *room = create_room(room_json, items, npcs);

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

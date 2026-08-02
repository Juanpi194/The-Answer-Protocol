#ifndef ROOMPARSER_HPP
#define ROOMPARSER_HPP

#include <list>
#include <map>
#include <string>
#include <libs/json.hpp>

class Room;
class Item;
class NPC;

class RoomParser
{
	public:
		// MODIFIED: ahora recibe los mapas de items/npcs ya parseados
		// (ver ItemParser/NPCParser), para poder engancharlos a la sala
		// que los referencia en su JSON ("items": [...], "npc": "..."").
		// Los mapas se pasan por referencia y se van "consumiendo" (erase)
		// según se asignan -- cada Item*/NPC* solo puede pertenecer a UNA
		// sala (Room los borra en su destructor).
		std::list<Room*>	parse(const nlohmann::json& rooms_json,
								  std::map<std::string, Item*>& items,
								  std::map<std::string, NPC*>& npcs);
};

#endif

#ifndef NPCPARSER_HPP
#define NPCPARSER_HPP

#include <map>
#include <string>
#include <libs/json.hpp>

class NPC;

class NPCParser
{
	public:
		// MODIFIED: devuelve un mapa (clave del JSON -> NPC*) en vez de una
		// lista plana -- así RoomParser puede saber a qué NPC concreto se
		// refiere cada sala, sin depender de que NPC/Character expongan un
		// id (no lo hacen). No-static, igual que ItemParser/RoomParser.
		std::map<std::string, NPC*>	parse(const nlohmann::json& json);
};

#endif

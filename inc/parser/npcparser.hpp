#ifndef NPCPARSER_HPP
#define NPCPARSER_HPP

#include <list>
#include <libs/json.hpp>

class NPC;

class NPCParser
{
	public:
		std::list<NPC*>	parse(const nlohmann::json& npcs_json);
};

#endif
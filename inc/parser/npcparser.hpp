#pragma once

#include <list>
#include <libs/json.hpp>

class NPC;

class NPCParser
{
	public:
		static std::list<NPC*>	parse(const nlohmann::json& npcs_json);
		static NPC				*build_npc(const std::string& id, const nlohmann::json& npc_json);
};
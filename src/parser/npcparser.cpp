#include "parser/npcparser.hpp"
#include <stdexcept>
#include "characters/QuestGiver.hpp"
#include "quests/Quest.hpp"
#include "utils/utils.hpp"

static NPC *build_npc(const std::string& id,
					  const std::string& name,
					  const std::string& description,
					  const std::string& role,
					  const std::string& quest_description)
{
	if (role == "quest_giver")
	{
		Quest quest(quest_description);
		return (new QuestGiver(id, name, description, quest));
	}

	//if (role == "narrator")
		//return (new Narrator(id, name, description));

	//if (role == "merchant")
		//return (new Merchant(id, name, description));

	//if (role == "trainer")
		//return (new Trainer(id, name, description));

	//if (role == "healer")
		//return (new Healer(id, name, description));

	log("NPC '" + id + "' has unsupported role '" + role + "'.",
		LogLevel::WARNING);

	return (nullptr);
}

std::list<NPC*> NPCParser::parse(const nlohmann::json& json)
{
	std::list<NPC*> npcs;

	if (!json.is_object() && !json.is_array())
		return (npcs);

	if (json.is_object())
	{
		for (auto it = json.begin(); it != json.end(); ++it)
		{
			const std::string id = "npc." + it.key();
			const std::string name = it.value()["name"];
			const std::string description = it.value()["description"];
			const std::string role = it.value()["role"];
			const std::string quest_description =
				it.value().value("quest_description", description);

			try
			{
				NPC *npc = build_npc(id, name, description,
									role, quest_description);

				if (npc != nullptr)
					npcs.push_back(npc);
			}
			catch (const std::exception& e)
			{
				log("Could not create NPC '" + id + "': " + e.what(),
					LogLevel::WARNING);
			}
		}
	}

	if (json.is_array())
	{
		for (const auto& npc_json : json)
		{
			const std::string id = "npc." + npc_json["id"].get<std::string>();
			const std::string name = npc_json["name"];
			const std::string description = npc_json["description"];
			const std::string role = npc_json["role"];
			const std::string quest_description =
				npc_json.value("quest_description", description);

			try
			{
				NPC *npc = build_npc(id, name, description,
									role, quest_description);

				if (npc != nullptr)
					npcs.push_back(npc);
			}
			catch (const std::exception& e)
			{
				log("Could not create NPC '" + id + "': " + e.what(),
					LogLevel::WARNING);
			}
		}
	}

	return (npcs);
}
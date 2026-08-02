#include "parser/npcparser.hpp"
#include <stdexcept>
#include "characters/QuestGiver.hpp"
#include "quests/Quest.hpp"
#include "utils/utils.hpp"

// MODIFIED: llamaba a Quest(quest_description) (1 arg) y a
// QuestGiver(id, name, description, quest) (4 args) -- ninguno de los dos
// constructores existe así. Quest pide (name, description); QuestGiver
// pide (name, description, quest, dialogues, gold_reward, item_reward), y
// no acepta un id (por eso NPCParser::parse ahora devuelve un mapa, para
// no perder la referencia al NPC por otro lado).
static NPC *build_npc(const std::string& id,
					  const std::string& name,
					  const std::string& description,
					  const std::string& role,
					  const std::string& quest_description)
{
	if (role == "quest_giver")
	{
		Quest quest(name + "'s quest", quest_description);
		t_quest_dialogues dialogues{
			description,          // intro
			"Come back when you're done.", // already_given
			"Thank you for your help!"     // finished
		};
		return (new QuestGiver(name, description, quest, dialogues, 0, nullptr));
	}

	// if (role == "narrator")
	// 	return (new Narrator(id, name, description));

	// if (role == "merchant")
	// 	return (new Merchant(id, name, description));

	//if (role == "trainer")
		//return (new Trainer(id, name, description));

	//if (role == "healer")
		//return (new Healer(id, name, description));

	log("NPC '" + id + "' has unsupported role '" + role + "'.",
		LogLevel::WARNING);

	return (nullptr);
}

std::map<std::string, NPC*> NPCParser::parse(const nlohmann::json& json)
{
	std::map<std::string, NPC*> npcs;

	if (!json.is_object())
		return (npcs);

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
				npcs[it.key()] = npc;
		}
		catch (const std::exception& e)
		{
			log("Could not create NPC '" + id + "': " + e.what(),
				LogLevel::WARNING);
		}
	}

	return (npcs);
}

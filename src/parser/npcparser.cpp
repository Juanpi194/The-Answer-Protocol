#include "parser/npcparser.hpp"
#include <stdexcept>
#include "characters/Enchanter.hpp"
#include "characters/Merchant.hpp"
#include "characters/Narrator.hpp"
#include "characters/QuestGiver.hpp"
#include "factories/ItemFactory.hpp"
#include "factories/NpcPacificFactory.hpp"
#include "quests/Quest.hpp"
#include "utils/utils.hpp"

static ObjectiveType string_to_objective_type(const std::string& type)
{
	if (type == "DEFEAT_ENEMY")
		return (ObjectiveType::DEFEAT_ENEMY);

	if (type == "COLLECT_ITEM")
		return (ObjectiveType::COLLECT_ITEM);

	if (type == "TALK_TO_NPC")
		return (ObjectiveType::TALK_TO_NPC);

	if (type == "REACH_AREA")
		return (ObjectiveType::REACH_AREA);

	if (type == "EARN_GOLD")
		return (ObjectiveType::EARN_GOLD);

	if (type == "REACH_LEVEL")
		return (ObjectiveType::REACH_LEVEL);

	throw std::invalid_argument(
		"Unknown objective type '" + type + "'.");
}


static t_quest_objective build_objective(
	const nlohmann::json& objective_json)
{
	t_quest_objective objective;

	objective.type = string_to_objective_type(
		objective_json["type"]);

	objective.target = objective_json.value("target", "");

	objective.amount = objective_json.value("amount", 0u); //0u representa el número 0 como unsigned int, para que coincida con el tipo de la variable amount.

	return (objective);
}


static Quest build_quest(const nlohmann::json& quest_json)
{
	Item *item_reward = nullptr;

	if (quest_json.contains("item_reward")
		&& !quest_json["item_reward"].is_null())
	{
		item_reward = ItemFactory::create_from_name(
			quest_json["item_reward"]);
	}

	return (
		Quest(
			quest_json["name"],
			quest_json["description"],
			quest_json.value("gold_reward", 0u),
			item_reward,
			build_objective(quest_json["objective"])
		)
	);
}


static t_quest_dialogues build_dialogues(
	const nlohmann::json& dialogues_json)
{
	t_quest_dialogues dialogues;

	dialogues.intro =
		dialogues_json.value("intro", "");

	dialogues.already_given =
		dialogues_json.value("already_given", "");

	dialogues.finished =
		dialogues_json.value("finished", "");

	return (dialogues);
}


static NPC *build_narrator(const nlohmann::json& npc_json)
{
	const std::string name = npc_json["name"];
	const std::string description = npc_json["description"];
	const std::string sentence = npc_json.value("sentence", "");

	return (
		NpcPacificFactory::create_narrator(
			name,
			description,
			sentence
		)
	);
}


static NPC *build_enchanter(
	const std::string& id,
	const nlohmann::json& npc_json)
{
	const std::string name = npc_json["name"];
	const std::string description = npc_json["description"];
	const std::string type = npc_json.value("type", "basic");

	if (type == "basic")
	{
		return (
			NpcPacificFactory::create_enchanter_basic(
				name,
				description
			)
		);
	}

	if (type == "advanced")
	{
		return (
			NpcPacificFactory::create_enchanter_advanced(
				name,
				description
			)
		);
	}

	log(
		"NPC '" + id
		+ "' has unsupported enchanter type '"
		+ type + "'.",
		LogLevel::WARNING);

	return (nullptr);
}


static NPC *build_merchant(
	const std::string& id,
	const nlohmann::json& npc_json)
{
	const std::string name = npc_json["name"];
	const std::string description = npc_json["description"];
	const std::string type = npc_json.value("type", "basic");

	if (type == "basic")
	{
		return (
			NpcPacificFactory::create_merchant_basic(
				name,
				description
			)
		);
	}

	if (type == "advanced")
	{
		return (
			NpcPacificFactory::create_merchant_advanced(
				name,
				description
			)
		);
	}

	log(
		"NPC '" + id
		+ "' has unsupported merchant type '"
		+ type + "'.",
		LogLevel::WARNING);

	return (nullptr);
}


static NPC *build_quest_giver(const nlohmann::json& npc_json)
{
	const std::string name = npc_json["name"];
	const std::string description = npc_json["description"];

	Quest quest = build_quest(npc_json["quest"]);
	t_quest_dialogues dialogues = build_dialogues(
		npc_json["dialogues"]);

	return (
		NpcPacificFactory::create_quest_giver(
			name,
			description,
			quest,
			dialogues
		)
	);
}


NPC *NPCParser::build_npc(
	const std::string& id,
	const nlohmann::json& npc_json)
{
	const std::string role = npc_json["role"];

	if (role == "narrator")
		return (build_narrator(npc_json));

	if (role == "enchanter")
		return (build_enchanter(id, npc_json));

	if (role == "merchant")
		return (build_merchant(id, npc_json));

	if (role == "quest_giver")
		return (build_quest_giver(npc_json));

	log(
		"NPC '" + id
		+ "' has unsupported role '"
		+ role + "'.",
		LogLevel::WARNING);

	return (nullptr);
}


static void add_npc(
	std::list<NPC*>& npcs,
	const std::string& id,
	const nlohmann::json& npc_json)
{
	try
	{
		NPC *npc = NPCParser::build_npc(id, npc_json);

		if (npc != nullptr)
			npcs.push_back(npc);
	}
	catch (const std::exception& e)
	{
		log(
			"Could not create NPC '"
			+ id + "': "
			+ e.what(),
			LogLevel::WARNING);
	}
}


std::list<NPC*> NPCParser::parse(const nlohmann::json& json)
{
	std::list<NPC*> npcs;

	if (!json.is_object() && !json.is_array())
		return (npcs);

	if (json.is_object())
	{
		for (nlohmann::json::const_iterator it = json.begin();
			it != json.end();
			++it)
		{
			add_npc(
				npcs,
				"npc." + it.key(),
				it.value());
		}
	}

	if (json.is_array())
	{
		for (const nlohmann::json& npc : json)
		{
			const std::string id = "npc." + npc["id"].get<std::string>();

			add_npc(npcs, id, npc);
		}
	}

	return (npcs);
}
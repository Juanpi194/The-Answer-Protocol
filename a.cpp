// #include <iostream>

// #include "parser/jsonconfig.hpp"
// #include "parser/itemparser.hpp"
// #include "parser/npcparser.hpp"
// #include "parser/roomparser.hpp"

// #include "characters/NPC.hpp"
// #include "items/Item.hpp"
// #include "world/Room.hpp"

// static int	g_failures = 0;

// static void	report(const std::string& test_name,
// 	bool success,
// 	const std::string& detail)
// {
// 	std::cout << "[";

// 	if (success)
// 		std::cout << "OK  ";
// 	else
// 		std::cout << "FAIL";

// 	std::cout << "] " << test_name;

// 	if (!detail.empty())
// 		std::cout << " -> " << detail;

// 	std::cout << std::endl;

// 	if (!success)
// 		++g_failures;
// }

// static bool	load_json(const std::string& path, nlohmann::json& data)
// {
// 	try
// 	{
// 		JsonConfig	config;

// 		data = config.load_json(path);

// 		report("JsonConfig::load_json(" + path + ")", true, "");

// 		return (true);
// 	}
// 	catch (const std::exception& e)
// 	{
// 		report("JsonConfig::load_json(" + path + ")", false, e.what());

// 		return (false);
// 	}
// }

// static void	parse_rooms(const std::string& path,
// 	const nlohmann::json& data)
// {
// 	try
// 	{
// 		RoomParser			room_parser;
// 		std::list<Room*>	rooms;

// 		rooms = room_parser.parse(data["rooms"]);

// 		report(
// 			"RoomParser::parse(" + path + ")",
// 			rooms.size() == data["rooms"].size(),
// 			std::to_string(rooms.size()) + " rooms creadas de "
// 			+ std::to_string(data["rooms"].size()));

// 		for (Room *room : rooms)
// 			delete room;
// 	}
// 	catch (const std::exception& e)
// 	{
// 		report("RoomParser::parse(" + path + ")", false, e.what());
// 	}
// }

// static void	parse_items(const std::string& path,
// 	const nlohmann::json& data)
// {
// 	try
// 	{
// 		ItemParser			item_parser;
// 		std::list<Item*>	items;

// 		items = item_parser.parse(data["items"]);

// 		report(
// 			"ItemParser::parse(" + path + ")",
// 			items.size() == data["items"].size(),
// 			std::to_string(items.size()) + " items creados de "
// 			+ std::to_string(data["items"].size()));

// 		for (Item *item : items)
// 			delete item;
// 	}
// 	catch (const std::exception& e)
// 	{
// 		report("ItemParser::parse(" + path + ")", false, e.what());
// 	}
// }

// static void	parse_npcs(const std::string& path,
// 	const nlohmann::json& data)
// {
// 	if (!data.contains("npcs"))
// 	{
// 		report(
// 			"NPCParser::parse(" + path + ")",
// 			false,
// 			"No existe la clave 'npcs'.");

// 		return ;
// 	}

// 	try
// 	{
// 		NPCParser			npc_parser;
// 		std::list<NPC*>	npcs;

// 		npcs = npc_parser.parse(data["npcs"]);

// 		report(
// 			"NPCParser::parse(" + path + ")",
// 			npcs.size() == data["npcs"].size(),
// 			std::to_string(npcs.size()) + " npcs creados de "
// 			+ std::to_string(data["npcs"].size()));

// 		for (NPC *npc : npcs)
// 			delete npc;
// 	}
// 	catch (const std::exception& e)
// 	{
// 		report("NPCParser::parse(" + path + ")", false, e.what());
// 	}
// }

// static void	test_json_file(const std::string& path)
// {
// 	nlohmann::json	data;

// 	std::cout << "========== " << path << " ==========" << std::endl;

// 	if (!load_json(path, data))
// 		return ;

// 	parse_rooms(path, data);
// 	parse_items(path, data);
// 	parse_npcs(path, data);

// 	std::cout << std::endl;
// }

// int	main(void)
// {
// 	std::cout << "========== TESTS JSON ==========" << std::endl << std::endl;

// 	test_json_file("newjson.json");

// 	std::cout << std::endl;

// 	if (g_failures == 0)
// 	{
// 		std::cout << "TODOS LOS TESTS HAN PASADO" << std::endl;
// 		return (0);
// 	}

// 	std::cout << g_failures << " TEST(S) HAN FALLADO" << std::endl;
// 	return (1);
// }

//  c++ -Wall -Wextra
//  -std=c++17 a.cpp src/parser/jsonconfig.cpp src/parser/roomparser.cpp src/parser
//  /itemparser.cpp src/parser/npcparser.cpp src/world/Room.cpp src/items/Item.cpp
//  src/items/Consumable.cpp src/characters/Character.cpp src/characters/NPC.cpp sr
//  c/characters/QuestGiver.cpp src/quests/Quest.cpp src/utils/log.cpp src/utils/co
//  lors.cpp src/utils/string_utils.cpp src/utils/globals.cpp -Iinc -o parser_test

#include <iostream>
#include "commands/commandparser.hpp"

static void report(const std::string& name, bool ok)
{
    std::cout << (ok ? "[OK  ] " : "[FAIL] ") << name << std::endl;
}

static void test_valid_command(
    const std::string& input,
    CommandType expected_type,
    size_t expected_args)
{
    CommandParser parser;

    try
    {
        Command cmd = parser.parse(input);

        bool ok = (cmd.type == expected_type &&
                   cmd.args.size() == expected_args);

        report(input, ok);
    }
    catch (const std::exception&)
    {
        report(input, false);
    }
}

static void test_invalid_command(const std::string& input)
{
    CommandParser parser;

    try
    {
        parser.parse(input);
        report(input, false);
    }
    catch (const CommandParseError&)
    {
        report(input, true);
    }
    catch (...)
    {
        report(input, false);
    }
}

int main(void)
{
    std::cout << "========== COMMAND PARSER ==========\n" << std::endl;

    std::cout << "----- COMANDOS VALIDOS -----" << std::endl;

    test_valid_command("LOOK", CommandType::LOOK, 0);

    test_valid_command("MOVE NORTH",
        CommandType::MOVE, 1);

    test_valid_command("CONNECT Alice",
        CommandType::CONNECT, 1);

    test_valid_command("TAKE Iron Sword",
        CommandType::TAKE, 1);

    test_valid_command("CHAT ROOM Hola equipo",
        CommandType::CHAT, 2);

    test_valid_command("ATTACK",
        CommandType::ATTACK, 0);

    test_valid_command("DEFEND",
        CommandType::DEFEND, 0);

    test_valid_command("FLEE",
        CommandType::FLEE, 0);

    test_valid_command("FIGHT",
        CommandType::FIGHT, 0);

    test_valid_command("CONSUME Apple",
        CommandType::CONSUME, 1);

    test_valid_command("GROUP",
        CommandType::GROUP, 0);

    test_valid_command("GROUP LEAVE",
        CommandType::GROUP, 1);

    test_valid_command("GROUP INVITE Alice",
        CommandType::GROUP, 2);

    std::cout << "\n----- COMANDOS INVALIDOS -----" << std::endl;

    test_invalid_command("");

    test_invalid_command("LOOK hola");

    test_invalid_command("MOVE");

    test_invalid_command("ATTACK Goblin");

    test_invalid_command("DEFEND Goblin");

    test_invalid_command("FLEE now");

    test_invalid_command("FIGHT Goblin");

    test_invalid_command("CONSUME");

    test_invalid_command("CONSUME Apple Pie");

    test_invalid_command("CHAT ROOM");

    test_invalid_command("CHAT CASA Hola");

    test_invalid_command("GROUP LEAVE Pepe");

    test_invalid_command("PATATA");

    return 0;
}

//[OK  ] MOVE
//No significa que el comando sea válido, sino que el parser ha hecho lo que esperabas: rechazarlo.
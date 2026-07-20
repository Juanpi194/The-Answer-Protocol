#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <ctime>

#ifndef WOW
bool debug_mode = false;
#else
bool debug_mode = true;
#endif

#include "utils/utils.hpp"
#include "quests/Quest.hpp"
#include "battle/Battle.hpp"
#include "characters/Character.hpp"
#include "characters/Enchanter.hpp"
#include "characters/Player.hpp"
#include "characters/Fighter.hpp"
#include "characters/Goblin.hpp"
#include "characters/Merchant.hpp"
#include "characters/QuestGiver.hpp"
#include "characters/Vendor.hpp"
#include "enchantments/Enchantment.hpp"
#include "enchantments/Flame.hpp"
#include "items/Chest.hpp"
#include "items/ChestKey.hpp"
#include "items/Consumable.hpp"
#include "items/IronArmor.hpp"
#include "items/IronSword.hpp"
#include "items/Item.hpp"
#include "items/ItemFactory.hpp"
#include "libs/json.hpp"
#include "quests/Quest.hpp"
#include "server/PlayerConnection.hpp"
#include "server/Server.hpp"
#include "server/ServerOwner.hpp"
#include "world/Room.hpp"
#include "world/World.hpp"

// TODO: DELETE THIS SHIT
static void	pruebitas(PlayerConnection& player_connection)
{
	// No server
	std::cout << "No server will be started" << std::endl;

	Player	*player = &player_connection.get_player();
	// Pruebitas
	World	world("The Amazing World Of Gumball");

	Item	*item1 = ItemFactory::create_iron_sword();
	Item	*item2 = new IronSword();
	Item	*item3 = new IronArmor();
	Item	*item4 = new ChestKey();

	std::list<Item*>	empty_list = {};
	std::list<Item*>	item_list_2 = {item1, item2, item3, item4};
	std::list<Item*>	item_list_3 = {};

	Enchantment	*enchantment = new Flame();
	std::map<Enchantment*, unsigned int>	enchantments = {{enchantment, 10}};

	// TODO: SHOW THIS SHIT
	NPC			*enchanter = new Enchanter("Rocio", "She sucks so much.", enchantments);
	Goblin		*goblin = new Goblin();
	Quest		quest("The great great quest", "This quest is only a test.");
	QuestGiver	*quest_giver = new QuestGiver("Chrystian", "Que pereza no?", quest, {"Papu papu, tienes una llamada de la grasa", "En proceso", "Fin"}, 100, nullptr);

	Chest	*chest_1 = new Chest();

	Room	*room1 = world.get_spawn_room();
	Room	*room2 = new Room("room.adiosss", "Adios", "Pues no tengo ni idea machote", goblin, chest_1, item_list_2);
	Room	*room3 = new Room("room.quest", "Habitacion Quest", "Aqui te dan una quest jeje", quest_giver, nullptr, item_list_3);
	Room	*room4 = new Room("room.enchantment_room", "Magia Jeje", "Aqui encantas las cosas", enchanter, nullptr, empty_list);

	room1->add_player(player);

	std::cout << current_level << std::endl;
	// These give warning in purpose.
	if (!world.add_room(room2, room1, Direction::NORTH))
	{
		delete (room2);
		room2 = nullptr;
	}
	if (!world.add_room(room3, room2, Direction::NORTH))
	{
		delete (room3);
		room3 = nullptr;
	}
	if (!world.add_room(room4, room3, Direction::NORTH))
	{
		delete (room4);
		room4 = nullptr;
	}

	while (true)
	{
		Room		*player_room;
		std::string	json_format;
		std::string	answer;
		std::getline(std::cin, answer);
		if (std::cin.eof())
			throw std::runtime_error("Input stream closed (EOF).");
		if (std::cin.fail())
			throw std::runtime_error("Error reading input.");
		player_room = player->get_current_room();

		if (answer == "MOVE NORTH")
			player->move(Direction::NORTH);
		else if (answer == "MOVE SOUTH")
			player->move(Direction::SOUTH);
		else if (answer == "LOOK")
		{
			json_format = player_room->look();
			nlohmann::json	j = nlohmann::json::parse(json_format);
			std::ofstream	result_file("datos.json");
			result_file << j.dump(4);
			result_file.close();
		}
		else if (answer == "TALK")
		{
			NPC	*npc = player_room->get_NPC();
			if (npc)
				player->talk_with(*npc);
			else
				std::cout << "No hay npc en esta sala, tonto\n";
		}
		else if (answer == "BUY")
		{
			NPC	*npc = player_room->get_NPC();
			if (!npc)
				std::cout << "No hay npc en esta sala, tonto\n";
			else
			{
				Vendor	*vendor = dynamic_cast<Vendor*>(npc);
				if (!vendor)
					std::cout << "El npc de esta sala no es un vendor, tonto\n";
				else
					vendor->on_buy(*player, "Iron Sword");
			}
		}
		else if (answer == "TAKE")
			player->obtain_item("Iron Sword");
		else if (answer == "TAKE KEY")
			player->obtain_item("Chest Key");
		else if (answer == "DROP")
			player->drop_item("Iron Sword");
		else if (answer == "OPEN")
		{
			Chest	*chest = player_room->get_chest();
			if (chest && !chest->is_opened())
			{
				std::list<Item*> result = chest->interact(*player);
				if (result.size() == 0)
					std::cout << "No tienes llave picha.\n";
				for (Item *item: result)
					player_room->add_item(item);
			}
			else if (chest && chest->is_opened())
				std::cout << "El cofre está abierto, máquina\n";
			else
				std::cout << "No hay cofre en esta sala, tonto\n";
		}
		else if (answer == "ATTACK")
		{
			NPC	*npc = player_room->get_NPC();
			if (!npc)
				"No hay npc en esta sala, tonto\n";
			else
			{
				Fighter	*fighter = dynamic_cast<Fighter*>(npc);
				if (!fighter)
					std::cout << "El npc no es un fighter, tonto\n";
				else
				{
					if (!player->get_battle())
						player->set_battle(new Battle(*player, *fighter));
				}
			}
		}
		else if (answer == "FLEE")
		{
			if (!player->get_battle())
				std::cout << "No estás en ningún combate, tonto\n";
			else
			{
				delete (player->get_battle());
				player->set_battle(nullptr);
			}
		}
		else if (answer == "QUIT")
			break ;
		else
			std::cout << "Tonto\n";
		for (std::string& s: player->get_outbox())
			std::cout << s << std::endl;
	}
	std::cout << "Adios" << std::endl;
}

int main(void)
{
	// SHORT IDEA OF HOW TO MANAGE DEBUG, flag in top of this file
	PlayerConnection player_connection("Juanpi", -1, nullptr);

	srand(time(nullptr));
	if (debug_mode)
		pruebitas(player_connection);
	else
	{
		// Server
		ServerOwner owner("Marcos", nullptr);
		std::cout << "Server started" << std::endl;
	}
	return (0);
}

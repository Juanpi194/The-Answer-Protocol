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
#include "characters/Character.hpp"
#include "characters/Enchanter.hpp"
#include "characters/Player.hpp"
#include "characters/Fighter.hpp"
#include "characters/Goblin.hpp"
#include "characters/Merchant.hpp"
#include "characters/QuestGiver.hpp"
#include "enchantments/Enchantment.hpp"
#include "enchantments/Flame.hpp"
#include "items/Chest.hpp"
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

	std::list<Item*>	item_list_1 = {item1, item2};
	std::list<Item*>	item_list_2 = {item3};
	std::list<Item*>	item_list_3 = {};

	Enchantment	*enchantment = new Flame();
	std::map<Enchantment*, unsigned int>	enchantments = {{enchantment, 10}};

	// TODO: SHOW THIS SHIT
	NPC			*enchanter = new Enchanter("Rocio", "She sucks so much.", enchantments);
	Goblin		*goblin = new Goblin();
	Quest		quest("The great great quest", "This quest is only a test.");
	QuestGiver	*quest_giver = new QuestGiver("Chrystian", "Que pereza no?", quest, {"Papu papu, tienes una llamada de la grasa", "En proceso", "Fin"}, 100, nullptr);

	Chest	*chest_1 = new Chest();

	Room	*room1 = new Room("room.holaaa", "Hola", "Pues no tengo ni idea tio", enchanter, nullptr, item_list_1);
	Room	*room2 = new Room("room.adiosss", "Adios", "Pues no tengo ni idea machote", goblin, chest_1, item_list_2);
	Room	*room3 = new Room("room.quest", "Habitacion Quest", "Aqui te dan una quest jeje", quest_giver, nullptr, item_list_3);

	room1->add_player(player);

	std::cout << current_level << std::endl;
	world.get_rooms().push_back(room1);
	world.add_room(room2, room1, Direction::NORTH);
	world.add_room(room3, room2, Direction::NORTH);

	while (true)
	{
		std::string	json_format;
		std::string	answer = player->receive_command();

		if (answer == "MOVE NORTH")
			player->move(Direction::NORTH);
		else if (answer == "MOVE SOUTH")
			player->move(Direction::SOUTH);
		else if (answer == "LOOK")
		{
			json_format = player->get_current_room()->look();
			nlohmann::json	j = nlohmann::json::parse(json_format);
			std::ofstream	result_file("datos.json");
			result_file << j.dump(4);
			result_file.close();
		}
		else if (answer == "INTERACT")
		{
			NPC	*npc = player->get_current_room()->get_NPC();
			if (npc)
				player->interact_with(*npc);
			else
				std::cout << "No hay npc en esta sala, tonto\n";
		}
		else if (answer == "TAKE")
			player->obtain_item("Iron Sword");
		else if (answer == "DROP")
			player->drop_item("Iron Sword");
		else if (answer == "OPEN")
		{
			Chest	*chest = player->get_current_room()->get_chest();
			if (chest && !chest->is_opened())
			{
				std::list<Item*> result = chest->interact(*player);
				if (result.size() == 0)
					std::cout << "No tienes llave picha.\n";
				for (Item *item: result)
					player->get_current_room()->add_item(item);
			}
			else if (chest && chest->is_opened())
				std::cout << "El cofre está abierto, máquina\n";
			else
				std::cout << "No hay cofre en esta sala, tonto\n";
		}
		else if (answer == "QUIT")
			break ;
		else
			std::cout << "Tonto\n";
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

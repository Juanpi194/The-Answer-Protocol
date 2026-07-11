#include <iostream>
#include <string>
#include <fstream>

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
#include "characters/Merchant.hpp"
#include "characters/QuestGiver.hpp"
#include "enchantments/Enchantment.hpp"
#include "enchantments/Flame.hpp"
#include "items/Item.hpp"
#include "items/IronSword.hpp"
#include "items/ItemFactory.hpp"
#include "items/Consumable.hpp"
#include "libs/json.hpp"
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

	std::list<Item*>	item_list = {item1, item2};
	std::list<Item*>	item_list_empty;

	Enchantment	*enchantment = new Flame();
	std::map<Enchantment*, unsigned int>	enchantments = {{enchantment, 10}};
	Enchanter	*enchanter = new Enchanter("Rocio", "She sucks so much.", enchantments);

	Room	*room1 = new Room("room.holaaa", "Hola", "Pues no tengo ni idea tio", enchanter, item_list);
	Room	*room2 = new Room("room.adiosss", "Adios", "Pues no tengo ni idea machote", nullptr, item_list_empty);

	room1->add_player(player);

	std::cout << current_level << std::endl;
	world.get_rooms().push_back(room1);
	world.add_room(room2, room1, Direction::NORTH);

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
		{
			player->obtain_item("Iron Sword");
		}
		else if (answer == "DROP")
		{
			player->drop_item("Iron Sword");
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

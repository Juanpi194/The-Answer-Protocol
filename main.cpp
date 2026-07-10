#include <iostream>
#include <string>

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
#include "server/PlayerConnection.hpp"
#include "server/Server.hpp"
#include "server/ServerOwner.hpp"
#include "world/Room.hpp"
#include "world/World.hpp"


int main(void)
{
	// SHORT IDEA OF HOW TO MANAGE DEBUG, flag in top of this file

	if (debug_mode)
	{
		// No server
		std::cout << "No server will be started" << std::endl;

		// Pruebitas
		World	world("The Amazing World Of Gumball");

		Item	*item1 = ItemFactory::create_iron_sword();
		std::cout << item1->get_id() << std::endl;
		std::cout << item1->get_name() << std::endl;
		std::cout << item1->get_description() << std::endl;

		Item	*item2 = new IronSword();
		std::cout << item2->get_id() << std::endl;
		std::cout << item2->get_name() << std::endl;
		std::cout << item2->get_description() << std::endl;

		std::list<Item*>	item_list = {item1, item2};
		std::list<Item*>	item_list_empty;

		Enchantment	*enchantment = new Flame();
		std::map<Enchantment*, unsigned int>	enchantments = {{enchantment, 10}};
		Enchanter	*enchanter = new Enchanter("Juanpi", "He sucks so much.", enchantments);
		std::cout << enchanter->get_id() << std::endl;
		std::cout << enchanter->get_name() << std::endl;
		std::cout << enchanter->get_description() << std::endl;

		Room	*room1 = new Room("room.holaaa", "Hola", "Pues no tengo ni idea tio", enchanter, item_list);
		Room	*room2 = new Room("room.adiosss", "Adios", "Pues no tengo ni idea machote", nullptr, item_list_empty);

		world.get_rooms().push_back(room1);
		world.add_room(room2, room1, Direction::NORTH);
	}
	else
	{
		// Server
		ServerOwner owner("Marcos", nullptr);
		std::cout << "Server started" << std::endl;
	}
	return (0);
}

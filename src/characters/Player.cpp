#include "characters/Player.hpp"

#include <iostream>

#include "utils/utils.hpp"
#include "battle/Battle.hpp"
#include "characters/Enemy.hpp"
#include "enchantments/Enchantment.hpp"
#include "items/Item.hpp"
#include "server/PlayerConnection.hpp"
#include "world/Room.hpp"

// Constructors ---------------------------------------------------------------

Player::Player(const std::string& name):
	Character(name),
	Fighter(name, {1, 10, 10, 10, 10}),
	gold(STARTING_GOLD),
	battle(nullptr)
{
}

Player::~Player(void)
{
	if (battle)
		delete (battle);
	for (Enchantment *enchantment: enchantment_list)
		delete (enchantment);
}

// Getters and setters --------------------------------------------------------

unsigned int					Player::get_gold(void) const noexcept
{
	return (gold);
}

const std::list<std::string>&	Player::get_beaten_enemies(void) const noexcept
{
	return (beaten_enemies_id);
}

const std::list<Quest>&			Player::get_quest_list(void) const noexcept
{
	return (quest_list);
}

Battle							*Player::get_battle(void) const noexcept
{
	return (battle);
}

void	Player::set_battle(Battle *battle) noexcept
{
	if (!battle)
		log("Player '" + get_name() + "'s battle set to nullptr.", LogLevel::INFO);
	this->battle = battle;
}

// Utils ----------------------------------------------------------------------

// Items --

bool		Player::obtain_item(Item *item) noexcept
{
	if (!item)
		return (log("Player '" + get_name() + "' tried to obtain a nullptr item.", LogLevel::INFO), false);
	if (!current_room)
		return (log("Player '" + get_name() + "' cannot obtain an item with nullptr current room.", LogLevel::WARNING), false);
	for (Item *item_in_list: inventory.get_items())
	{
		if (item_in_list == item)
			return (log("Player '" + get_name() + "' tried to obtain '" + item->get_id() + "', which is already in its item list.", LogLevel::WARNING), false);
	}

	// Item found, now it will get removed from the room and added to the player.
	try
	{
		current_room->remove_item(item);
		inventory.add_item(item);
	}
	catch (const std::invalid_argument& e)
	{
		return (log(e.what(), LogLevel::ERROR), false);
	}
	return (log(item->get_name() + " was obtained from the room '" + current_room->get_name() + "'.", LogLevel::DEBUG), true);
}

bool		Player::obtain_item(const std::string& item_name) noexcept
{
	std::string	cleaned_name = item_name;
	Item		*item_found;

	item_found = nullptr;
	if (!current_room)
		return (log("Player '" + get_name() + "' cannot obtain an item with nullptr current room.", LogLevel::WARNING), false);
	trim_str(cleaned_name, false);
	for (Item *item: current_room->get_items())
	{
		if (cleaned_name == item->get_name() || cleaned_name == item->get_id())
			item_found = item;
	}
	if (!item_found)
		return (log("Couldn't find '" + item_name + "' in the room.", LogLevel::INFO), false);

	// Item found, now it will get removed from the room and added to the player.
	try
	{
		current_room->remove_item(item_found);
		inventory.add_item(item_found);
	}
	catch (const std::invalid_argument& e)
	{
		return (log(e.what(), LogLevel::ERROR), false);
	}
	return (log(item_found->get_name() + " was obtained from the room '" + current_room->get_name() + "'.", LogLevel::DEBUG), true);
}

bool		Player::drop_item(Item *item) noexcept
{
	// ? Review: Check logic and maybe add mutex...
	bool	found;

	found = false;
	if (!item)
		return (log("Player '" + get_name() + "' tried to drop a nullptr item.", LogLevel::INFO), false);
	if (!current_room)
		return (log("Player '" + get_name() + "' cannot drop an item with nullptr current room.", LogLevel::WARNING), false);
	for (Item *item_in_list: inventory.get_items())
	{
		if (item_in_list == item)
			found = true;
	}
	if (!found)
		return (log("Item '" + item->get_id() + "' does not exist in player's item list.", LogLevel::WARNING), false);

	// Item found, now it will get removed from the player and added to the room.
	try
	{
		current_room->add_item(item);
		inventory.remove_item(item);
	}
	catch (const std::invalid_argument& e)
	{
		return (log(e.what(), LogLevel::ERROR), false);
	}
	return (log(item->get_name() + " was dropped into the room '" + current_room->get_name() + "'.", LogLevel::DEBUG), true);
}

bool		Player::drop_item(const std::string& item_name) noexcept
{
	std::string	cleaned_name = item_name;
	Item		*item_found;

	item_found = nullptr;
	if (!current_room)
		return (log("Player '" + get_name() + "' cannot drop an item with nullptr current room.", LogLevel::WARNING), false);
	trim_str(cleaned_name, false);
	for (Item *item: inventory.get_items())
	{
		if (cleaned_name == item->get_name() || cleaned_name == item->get_id())
			item_found = item;
	}
	if (!item_found)
		return (log("Item '" + item_name + "' does not exist in player's item list.", LogLevel::WARNING), false);

	// Item found, now it will get removed from the player and added to the room.
	try
	{
		current_room->add_item(item_found);
		inventory.remove_item(item_found);
	}
	catch (const std::invalid_argument& e)
	{
		return (log(e.what(), LogLevel::ERROR), false);
	}
	return (log(item_found->get_name() + " was dropped into the room '" + current_room->get_name() + "'.", LogLevel::DEBUG), true);
}

// Enchantments --

void		Player::add_enchantment(Enchantment *enchantment) noexcept
{
	assert(enchantment != nullptr && "Cannot add a nullptr to the enchantment list.");
	enchantment_list.push_back(enchantment);
	log("Player '" + get_name() + "' received the '" + enchantment->get_name() + "' enchantment.", LogLevel::DEBUG);
}

void		Player::consume_enchantment(Enchantment& enchantment)
{
	Enchantment	*enchantment_found;

	enchantment_found = nullptr;
	for (Enchantment *enchantment_in_list: enchantment_list)
	{
		if (enchantment_in_list == &enchantment)
			enchantment_found = enchantment_in_list;
	}
	if (!enchantment_found)
		throw std::invalid_argument("Enchantment to consume is not in the player's item list.");

	// Removing the item
	enchantment_list.remove(enchantment_found);
	delete (enchantment_found);
}

Enchantment	*Player::find_enchantment_by_name(const std::string& enchantment_name) const
{
	for (Enchantment *enchantment_in_list: enchantment_list)
	{
		if (enchantment_in_list->get_name() == enchantment_name)
			return (enchantment_in_list);
	}
	return (nullptr);
}

// Quests --

bool		Player::obtain_quest(Quest& quest) noexcept
{
	for (Quest& quest_in_list: quest_list)
	{
		if (quest_in_list.get_name() == quest.get_name())
			return (log("Player '" + get_name() + "' already has the quest '" + quest.get_name() + "'.", LogLevel::WARNING), false);
	}
	quest_list.push_back(quest);
	return (log("Player '" + get_name() + "' received the quest '" + quest.get_name() + "'.", LogLevel::DEBUG), true);
}

// Gold --

bool		Player::spend_gold(unsigned int quantity) noexcept
{
	if (quantity > gold)
		return (log("Player '" + get_name() + "' does not have enough gold to spend.", LogLevel::INFO), false);
	gold -= quantity;
	return (log("Player '" + get_name() + "' spent " + std::to_string(quantity) + " gold.", LogLevel::INFO), true);
}

void		Player::lose_gold(unsigned int quantity) noexcept
{
	if (quantity > gold)
		gold = 0;
	else
		gold -= quantity;
	log("Player '" + get_name() + "' lost " + std::to_string(quantity) + " gold.", LogLevel::INFO);
}

// Location --

bool		Player::move(Direction direction) noexcept
{
	// ? REVIEW: Needed mutex?
	Room	*adjacent;

	// Checking room
	if (!current_room)
		return (log("Player '" + get_name() + "' is not at any room, it cannot move.", LogLevel::INFO), false);
	if (current_room->get_adjacent_rooms().count(direction) == 0)
		return (log("Player '" + get_name() + "' tried to move to a non existing room.", LogLevel::INFO), false);

	// Moving from one room to another
	adjacent = current_room->get_adjacent_rooms().at(direction);
	try
	{
		current_room->remove_player(this);
		adjacent->add_player(this);
		current_room = adjacent;
	}
	catch (const std::invalid_argument& e)
	{
		return (log(e.what(), LogLevel::ERROR), false);
	}
	return (log("Player '" + get_name() + "' entered '" + current_room->get_name() + "'.", LogLevel::DEBUG), true);
}

void		Player::respawn(Room *destination)
{
	if (!current_room)
		throw std::runtime_error("Tried to respawn player '" + get_name() + "' but he is not located anywhere.");
	current_room->remove_player(this);
	destination->add_player(this);
	stats.current_hp = (stats.hp / 2);
}

// Fight --

bool		Player::is_enemy_beaten(Enemy *enemy) noexcept
{
	if (!enemy)
		return (log("Cannot check nullptr enemy.", LogLevel::ERROR), false);
	for (const std::string& beaten_enemy_id: beaten_enemies_id)
	{
		if (beaten_enemy_id == enemy->get_id())
			return (true);
	}
	return (false);
}

void		Player::choose_action(void)
{
	// TODO: Request the player an action to perform...
}

void		Player::attack(Fighter& target) noexcept
{
	// TODO: Attack logic...
}

FighterType	Player::get_type(void) const noexcept
{
	return (FighterType::Player);
}

// Interactions --

void		Player::talk_with(Character& character)
{
	// TODO: Logic...
	log("Player '" + get_name() + "' interacted with '" + character.get_name() + "'.", LogLevel::DEBUG);
	character.on_talk(*this);
}

void		Player::on_talk(Player& player) noexcept
{
	// TODO: Logic...
}

// User --

void					Player::send_to_outbox(const std::string& msg)
{
	std::string					cleared_msg = msg;

	trim_str(cleared_msg, false);
	std::lock_guard<std::mutex>	lock(outbox_mtx);
	if (!cleared_msg.empty())
		outbox.push_back(msg);
	else
		log("Empty message sent from '" + get_name() + "'.", LogLevel::WARNING);
}

std::list<std::string>	Player::drain_outbox(void)
{
	std::lock_guard<std::mutex>	lock(outbox_mtx);

	std::list<std::string>	out;
	std::swap(out, outbox);
	return (out);
}

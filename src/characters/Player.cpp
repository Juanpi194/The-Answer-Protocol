#include "characters/Player.hpp"

#include <iostream>

#include "utils/utils.hpp"
#include "server/PlayerConnection.hpp"
#include "world/Room.hpp"

// Constructors ---------------------------------------------------------------

Player::Player(const std::string& name):
	Character(name),
	Fighter(name, {1, 10, 10, 10, 10}),
	// player_connection(player_connection),
	// ? REVIEW: Check client fd
	// client_fd(-1),
	gold(0)
{
	// FIXME: Decide if a player must have a PlayerConnection attached to them or not (debug mode)
	// if (!player_connection)
	// 	throw std::invalid_argument("Each player must have a player connection.");
}

// Getters and setters --------------------------------------------------------

unsigned int				Player::get_gold(void) const noexcept
{
	return (gold);
}

// std::list<Enemy*>&			Player::get_beaten_enemies(void) noexcept
// {
// 	return (beaten_enemies);
// }

const std::list<Enemy*>&	Player::get_beaten_enemies(void) const noexcept
{
	return (beaten_enemies);
}

// std::list<Quest>&			Player::get_quest_list(void) noexcept
// {
// 	return (quest_list);
// }

const std::list<Quest>&		Player::get_quest_list(void) const noexcept
{
	return (quest_list);
}

std::list<std::string>&		Player::get_outbox(void) noexcept
{
	return (outbox);
}

// Utils ----------------------------------------------------------------------

// Items --

void		Player::add_item(Item *item) noexcept
{
	item_list.push_back(item);
	log("Player '" + get_name() + "' received '" + item->get_name() + "'.", LogLevel::INFO);
}

bool		Player::obtain_item(Item *item) noexcept
{
	if (!item)
		return (log("Player '" + get_name() + "' tried to obtain a nullptr item.", LogLevel::INFO), false);
	if (!current_room)
		return (log("Player '" + get_name() + "' cannot obtain an item with nullptr current room.", LogLevel::WARNING), false);
	for (Item *item_in_list: item_list)
	{
		if (item_in_list == item)
			return (log("Player '" + get_name() + "' tried to obtain '" + item->get_id() + "', which is already in its item list.", LogLevel::WARNING), false);
	}

	// Item found, now it will get removed from the room and added to the player.
	try
	{
		current_room->remove_item(item);
		item_list.push_back(item);
	}
	catch (const std::invalid_argument& e)
	{
		log(e.what(), LogLevel::ERROR);
		return (false);
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
		item_list.push_back(item_found);
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
	for (Item *item_in_list: item_list)
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
		item_list.remove(item);
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
	for (Item *item: item_list)
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
		item_list.remove(item_found);
	}
	catch (const std::invalid_argument& e)
	{
		return (log(e.what(), LogLevel::ERROR), false);
	}
	return (log(item_found->get_name() + " was dropped into the room '" + current_room->get_name() + "'.", LogLevel::DEBUG), true);
}

void		Player::consume_item(Item& item)
{
	Item	*item_found;

	item_found = nullptr;
	for (Item *item_in_list: item_list)
	{
		if (item_in_list == &item)
			item_found = item_in_list;
	}
	if (!item_found)
		throw std::invalid_argument("Item to consume is not in the player's item list.");

	// Removing the item
	item_list.remove(item_found);
	delete (item_found);
}

void		Player::buy_item(const Merchant& merchant, Item *item)
{
	// TODO: Logic...
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
	log("Player '" + get_name() + "' lost " + std::to_string(gold) + " gold.", LogLevel::INFO);
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

// Fight --

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

void		Player::send_to_client(const std::string& msg)
{
	std::string	cleared_msg = msg;

	trim_str(cleared_msg, false);
	if (cleared_msg.empty())
		throw std::invalid_argument("Cannot send an empty message to the client.");
	outbox.push_back(msg);
}

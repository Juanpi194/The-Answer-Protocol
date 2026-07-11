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
	client_fd(-1),
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

std::list<Enemy*>&			Player::get_beaten_enemies(void) noexcept
{
	return (beaten_enemies);
}

const std::list<Enemy*>&	Player::get_beaten_enemies(void) const noexcept
{
	return (beaten_enemies);
}

std::list<Quest>&			Player::get_quest_list(void) noexcept
{
	return (quest_list);
}

const std::list<Quest>&		Player::get_quest_list(void) const noexcept
{
	return (quest_list);
}

// Utils ----------------------------------------------------------------------

void		Player::obtain_item(Item *item)
{
	// TODO: Logic...
}

void		Player::drop_item(Item *item)
{
	// TODO: Logic...
}

void		Player::buy_item(const Merchant& merchant, Item *item)
{
	// TODO: Logic...
}

bool		Player::move(Direction direction) noexcept
{
	// ? REVIEW: Needed mutex?
	Room	*adyacent;

	// Checking room
	if (!current_room)
		return (log("Player '" + get_name() + "' is not at any room, it cannot move.", LogLevel::INFO), false);
	if (current_room->get_adyacent_rooms().count(direction) == 0)
		return (log("Player '" + get_name() + "' tried to move to a non existing room.", LogLevel::INFO), false);

	// Moving from one room to another
	adyacent = current_room->get_adyacent_rooms()[direction];
	current_room->remove_player(this);
	current_room = adyacent;
	current_room->add_player(this);
	return (log("Player '" + get_name() + "' entered '" + current_room->get_name() + "'.", LogLevel::DEBUG), true);
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

void		Player::interact_with(Character& character)
{
	// TODO: Logic...
	log("Player '" + get_name() + "' interacted with '" + character.get_name() + "'.", LogLevel::DEBUG);
	character.on_interact(*this);
}

void		Player::on_interact(Player& player)
{
	// TODO: Logic...
}

std::string	Player::receive_command(void)
{
	std::string	result;

	if (client_fd != -1)
	{
		// TODO: Server logic...
	}
	else
	{
		std::getline(std::cin, result);
		if (std::cin.eof())
			throw std::runtime_error("Input stream closed (EOF).");
		if (std::cin.fail())
			throw std::runtime_error("Error reading input.");
	}
	return (result);
}

void		Player::send_to_client(const std::string& msg) const
{
	std::string	cleared_msg = msg;
	
	trim_str(cleared_msg, false);
	if (cleared_msg.empty())
		throw std::invalid_argument("Cannot send an empty message to the client.");
	if (client_fd != -1)
	{
		// TODO: Server logic...
	}
	else
		std::cout << msg << std::endl;
}


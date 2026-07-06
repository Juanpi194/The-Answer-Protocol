#include "characters/Player.hpp"

#include <iostream>

#include "server/PlayerConnection.hpp"

// Constructors ---------------------------------------------------------------

Player::Player(const std::string& name):
	Character(name),
	Fighter(name, {10, 10}),
	// player_connection(player_connection),
	current_location(nullptr),
	gold(0)
{
	// FIXME: Decide if a player must have a PlayerConnection attached to them or not (debug mode)
	// if (!player_connection)
	// 	throw std::invalid_argument("Each player must have a player connection.");
}

// Getters and setters --------------------------------------------------------

Room						*Player::get_current_location(void) const noexcept
{
	return (current_location);
}

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

void	Player::obtain_item(Item *item)
{
	// TODO: Logic...
}

void	Player::drop_item(Item *item)
{
	// TODO: Logic...
}

void	Player::buy_item(const Merchant& merchant, Item *item)
{
	// TODO: Logic...
}

void	Player::move(void)
{
	// TODO: Change location through Zone adyacent zones, ...
}

void	Player::choose_action(void)
{
	// TODO: Request the player an action to perform...
}

void	Player::attack(Fighter& target) noexcept
{
	// TODO: Attack logic...
}

FighterType	Player::get_type(void) const noexcept
{
	return (FighterType::Player);
}

void	Player::interact_with(Character& character)
{
	// TODO: Logic...
	character.on_interact(*this);
}

void	Player::on_interact(Player& player)
{
	// TODO: Logic...
}

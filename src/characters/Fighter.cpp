#include "characters/Fighter.hpp"

#include <stdexcept>

#include "utils/utils.hpp"

bool	Fighter::validate_stats(t_stats stats)
{
	// TODO: Add validations
	return (true);
}

// Constructors ---------------------------------------------------------------

Fighter::Fighter(const std::string& name, t_stats stats):
	Character(name),
	armor(nullptr),
	weapon(nullptr),
	stats(stats),
	status(Status::HEALTHY)
{
	// TODO: Call validate_stats and finish all the stat validations.
	if (stats.hp < MIN_HP || stats.hp > MAX_HP || stats.strength < MIN_STRENGTH || stats.strength > MAX_STRENGTH)
		throw std::invalid_argument("Fighter stats are not valid.");
}

Fighter::Fighter(const Fighter& fighter):
	Character(fighter.get_name()),
	armor(fighter.armor),
	weapon(fighter.weapon),
	stats(fighter.stats),
	status(fighter.status)
{
	this->current_room = fighter.current_room;
}

Fighter::~Fighter(void)
{
	// TODO: Free all items in the list and armor and weapon if they exist.
}

// Getters and setters --------------------------------------------------------

Armor					*Fighter::get_armor(void) const noexcept
{
	return (armor);
}

Weapon					*Fighter::get_weapon(void) const noexcept
{
	return (weapon);
}

t_stats					Fighter::get_stats(void) const noexcept
{
	return (stats);
}

Status					Fighter::get_status(void) const noexcept
{
	return (status);
}

std::list<Item*>&		Fighter::get_item_list(void) noexcept
{
	return (item_list);
}

const std::list<Item*>&	Fighter::get_item_list(void) const noexcept
{
	return (item_list);
}

void	Fighter::set_armor(Armor *armor) noexcept
{
	if (!armor)
		log("Fighter '" + get_name() + "' received nullptr armor.", LogLevel::INFO);
	this->armor = armor;
}

void	Fighter::set_weapon(Weapon *weapon) noexcept
{
	if (!weapon)
		log("Fighter '" + get_name() + "' received nullptr weapon.", LogLevel::INFO);
	this->weapon = weapon;
}

void	Fighter::set_status(Status status) noexcept
{
	this->status = status;
}

// Utils ----------------------------------------------------------------------

void	Fighter::apply_status(Status status) noexcept
{
	// TODO: Logic...
}

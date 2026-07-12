#include "characters/Fighter.hpp"

#include <stdexcept>

#include "utils/utils.hpp"

bool	Fighter::validate_stats(t_stats stats)
{
	// TODO: Add validations
	if (stats.hp < MIN_HP || stats.hp > MAX_HP || stats.strength < MIN_STRENGTH || stats.strength > MAX_STRENGTH)
		return (false);
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
	if (!validate_stats(stats))
		throw std::invalid_argument("Fighter stats are not valid.");
}

Fighter::~Fighter(void)
{
	for (Item *item: item_list)
		delete (item);
	if (armor)
		delete (armor);
	if (weapon)
		delete (weapon);
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

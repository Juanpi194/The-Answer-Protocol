#include "characters/Fighter.hpp"

#include <stdexcept>

#include "battle/Battle.hpp"
#include "characters/Player.hpp"
#include "items/Armor.hpp"
#include "items/Consumable.hpp"
#include "items/Shield.hpp"
#include "items/SpecialEffectGear.hpp"
#include "items/Weapon.hpp"
#include "utils/utils.hpp"

std::string	status_to_string(const Status status) noexcept
{
	switch (status)
	{
		case Status::HEALTHY:
			return ("healthy");
		case Status::POISONED:
			return ("poisoned");
		case Status::BURNT:
			return ("burnt");
		case Status::FROZEN:
			return ("frozen");
		case Status::BLEEDING:
			return ("bleeding");
		case Status::CONFUSED:
			return ("confused");
		default:
			return ("unknown");
	}
}

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
	shield(nullptr),
	weapon(nullptr),
	last_action(FightAction::NONE),
	defending(false),
	stats(stats),
	status(Status::HEALTHY)
{
	if (!validate_stats(stats))
		throw std::invalid_argument("Fighter stats are not valid.");
}

Fighter::~Fighter(void)
{
	if (armor)
		delete (armor);
	if (shield)
		delete (shield);
	if (weapon)
		delete (weapon);
}

// Getters and setters --------------------------------------------------------

Armor				*Fighter::get_armor(void) const noexcept
{
	return (armor);
}

Shield				*Fighter::get_shield(void) const noexcept
{
	return (shield);
}

Weapon				*Fighter::get_weapon(void) const noexcept
{
	return (weapon);
}

FightAction			Fighter::get_last_action(void) const noexcept
{
	return (last_action);
}

bool				Fighter::is_defending(void) const noexcept
{
	return (defending);
}

t_stats				Fighter::get_stats(void) const noexcept
{
	return (stats);
}

Status				Fighter::get_status(void) const noexcept
{
	return (status);
}

Inventory&			Fighter::get_inventory(void) noexcept
{
	return (inventory);
}

void	Fighter::set_armor(Armor *armor) noexcept
{
	if (!armor)
		log("Fighter '" + get_name() + "' received nullptr armor.", LogLevel::INFO);
	this->armor = armor;
}

void	Fighter::set_shield(Shield *shield) noexcept
{
	if (!shield)
		log("Fighter '" + get_name() + "' received nullptr shield.", LogLevel::INFO);
	this->shield = shield;
}

void	Fighter::set_weapon(Weapon *weapon) noexcept
{
	if (!weapon)
		log("Fighter '" + get_name() + "' received nullptr weapon.", LogLevel::INFO);
	this->weapon = weapon;
}

void	Fighter::set_last_action(FightAction last_action) noexcept
{
	this->last_action = last_action;
}

void	Fighter::set_defending(bool defending) noexcept
{
	this->defending = defending;
}

void	Fighter::set_status(Status status) noexcept
{
	this->status = status;
}

// Utils ----------------------------------------------------------------------

void		Fighter::perform_action(Fighter& opponent, FightChoice choice)
{
	switch (choice.action)
	{
		case FightAction::ATTACK:
			attack(opponent);
			break;
		case FightAction::DEFEND:
			defend();
			break;
		case FightAction::CONSUME:
			assert(choice.consumable != nullptr && "CONSUME must select an item to consume.");
			consume(opponent, *choice.consumable);
			break;
		default:
			throw std::invalid_argument("Cannot perform that action from this method (by '" + get_name() + "').");
	}
}

void		Fighter::attack(Fighter& target) noexcept
{
	unsigned int		final_strength;
	SpecialEffectGear	*special_weapon;

	// If target is defending
	if (target.is_defending())
	{
		target.get_shield()->protect(target, *this);
		return;
	}

	// Calculating damage
	final_strength = (stats.current_strength / 2);
	if (weapon)
		final_strength += weapon->get_extra_damage() / 3;

	// Special effect
	special_weapon = dynamic_cast<SpecialEffectGear*>(weapon);
	if (special_weapon)
		special_weapon->special_effect(*this, target);

	// Receiving damage
	target.receive_damage(*this, final_strength);
}

void		Fighter::defend(void) noexcept
{
	if (shield)
		defending = true;
}

bool		Fighter::flee(Fighter& opponent) noexcept
{
	// TODO: Randomly choose, having in mind the opponent...
	return (true);
}

void		Fighter::consume(Fighter& opponent, Consumable& consumable) noexcept
{
	// TODO: Logic
}

void		Fighter::receive_damage(Fighter& attacker, unsigned int incoming_damage) noexcept
{
	unsigned int		final_defense;
	SpecialEffectGear	*special_armor;

	// Defense
	final_defense = (stats.current_defense / 2);
	if (armor)
	{
		if (incoming_damage > armor->get_damage_reduction())
			incoming_damage -= armor->get_damage_reduction();
		else
			incoming_damage = 1;
	}

	// Final damage
	if (incoming_damage > final_defense)
		incoming_damage -= final_defense;
	else
		incoming_damage = 1;

	// Setting hp
	if (stats.current_hp > incoming_damage)
		stats.current_hp -= incoming_damage;
	else
		stats.current_hp = 0;

	// Special effect
	special_armor = dynamic_cast<SpecialEffectGear*>(armor);
	if (special_armor)
		special_armor->special_effect(*this, attacker);
	log("'" + attacker.get_name() + "' hit '" + get_name() + "' for " + std::to_string(incoming_damage) + " damage.", LogLevel::INFO);
}

void		Fighter::apply_status(Status status) noexcept
{
	// TODO: Logic...
}

void		Fighter::heal(unsigned int amount) noexcept
{
	// TODO: Logic...
}

void		Fighter::change_stat(Stat stat, int stage) noexcept
{
	// TODO: Logic...
}

void		Fighter::restore_stat(Stat stat) noexcept
{
	if (stat == Stat::HP)
		stats.current_hp = stats.hp;
	else if (stat == Stat::STRENGTH)
		stats.current_strength = stats.strength;
	else if (stat == Stat::DEFENSE)
		stats.current_defense = stats.defense;
	else if (stat == Stat::SPEED)
		stats.current_speed = stats.speed;
}

void		Fighter::reset_stats(bool hp) noexcept
{
	if (hp)
		restore_stat(Stat::HP);
	restore_stat(Stat::STRENGTH);
	restore_stat(Stat::DEFENSE);
	restore_stat(Stat::SPEED);
}

std::string	Fighter::status_json(void) const noexcept
{
	std::string	result;

	result = "{";
	result += "\"current_hp\": " + std::to_string(stats.current_hp);
	result += ", ";
	result += "\"hp\": " + std::to_string(stats.hp);
	result += ", ";
	result += "\"current_strength\": " + std::to_string(stats.current_strength);
	result += ", ";
	result += "\"strength\": " + std::to_string(stats.strength);
	result += ", ";
	result += "\"current_defense\": " + std::to_string(stats.current_defense);
	result += ", ";
	result += "\"defense\": " + std::to_string(stats.defense);
	result += ", ";
	result += "\"current_speed\": " + std::to_string(stats.current_speed);
	result += ", ";
	result += "\"speed\": " + std::to_string(stats.speed);
	result += ", ";
	result += "\"status\": \"" + status_to_string(status) + "\"";
	result += "}";
	return (result);
}

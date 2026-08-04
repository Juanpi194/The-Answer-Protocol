#pragma once

#include "characters/Character.hpp"
#include "items/Inventory.hpp"

class Armor;
class Consumable;
class Shield;
class Weapon;

enum Status
{
	HEALTHY,
	POISONED,
	BURNT,
	FROZEN,
	BLEEDING,
	CONFUSED
};

/**
 * @brief	Casts the given status to string format.
 * @param	status	The status to cast.
 * @returns	The string format of `status`. If status is unknown,
 * 			the word 'unknown' will be returned.
 */
std::string	status_to_string(const Status status) noexcept;

/**
 * @param	level		The level of the fighter.
 * @param	hp			The hp of the fighter (health points).
 * @param	strength	The strength of the fighter (attack).
 * @param	defense		The defense of the fighter (defense).
 * @param	speed		The speed of the fighter (speed).
 */
struct t_stats
{
	unsigned int	level;
	unsigned int	hp;
	unsigned int	strength;
	unsigned int	defense;
	unsigned int	speed;
	unsigned int	current_hp;
	unsigned int	current_strength;
	unsigned int	current_defense;
	unsigned int	current_speed;
};

// ? REVIEW: Is this the best way to make this?
enum class FighterType
{
	Player,
	Enemy
};

enum class FightAction
{
	NONE,
	ATTACK,
	DEFEND,
	FLEE,
	CONSUME
};

/**
 * @brief	Struct with an action to perform and a possible item to consume.
 * @param	action	The action to perform.
 * @param	consumable	The item to consume in case the action is `CONSUME`.
 */
struct FightChoice
{
	FightAction action;
	Consumable	*consumable = nullptr;	// Only used if action == CONSUME
};

enum class Stat
{
	HP,
	STRENGTH,
	DEFENSE,
	SPEED
};

// Mixin / Abstract class with just pure virtual methods and attributes (not an interface)
class Fighter: public virtual Character
{
	private:
		Armor		*armor;
		Shield		*shield;
		Weapon		*weapon;
		FightAction	last_action;
		bool		defending;

		static constexpr unsigned int	MIN_HP = 5;
		static constexpr unsigned int	MAX_HP = 100;
		static constexpr unsigned int	MIN_STRENGTH = 1;
		static constexpr unsigned int	MAX_STRENGTH = 80;
		static constexpr unsigned int	MIN_DEFENSE = 1;
		static constexpr unsigned int	MAX_DEFENSE = 90;
		static constexpr unsigned int	MIN_SPEED = 1;
		static constexpr unsigned int	MAX_SPEED = 70;

		/**
		 * @brief	Validates if the provided stats for the fighter are valid.
		 * @note	This method should ONLY be called in the constructor.
		 */
		static bool	validate_stats(t_stats stats);
	protected:
		t_stats		stats;
		Status		status;
		Inventory	inventory;
	public:
		// Constructors -------------------------------------------------------

		// TODO: Add a way to create a fighter with specific items.

		Fighter(const std::string& name, t_stats stats);
		Fighter(const Fighter& fighter) = delete;
		virtual ~Fighter(void);

		// Operators ----------------------------------------------------------

		Fighter&	operator=(const Fighter& other) = delete;

		// Getters and setters ------------------------------------------------

		Armor				*get_armor(void) const noexcept;
		Shield				*get_shield(void) const noexcept;
		Weapon				*get_weapon(void) const noexcept;
		FightAction			get_last_action(void) const noexcept;
		bool				is_defending(void) const noexcept;
		t_stats				get_stats(void) const noexcept;
		Status				get_status(void) const noexcept;
		Inventory&			get_inventory(void) noexcept;

		void	set_armor(Armor *armor) noexcept;
		void	set_shield(Shield *shield) noexcept;
		void	set_weapon(Weapon *weapon) noexcept;
		void	set_last_action(FightAction last_action) noexcept;
		void	set_defending(bool defending) noexcept;
		void	set_status(Status status) noexcept;

		// Utils --------------------------------------------------------------

		virtual FighterType	get_type() const noexcept = 0;

		/**
		 * @brief	Performs the selected action, done by the user. Only
		 * 			`ATTACK`, `DEFEND`, `CONSUME` are supported. `FLEE`
		 * 			is not checked here.
		 * @param	opponent	`user`'s opponent.
		 * @param	choice		
		 * @throws	`std::invalid_argument` if the action to perform is not available.
		 */
		void				perform_action(Fighter& opponent, FightChoice choice);

		/**
		 * @brief	Logic to perform when `CONSUME` action is selected.
		 * @param	opponent	The user's opponent.
		 * @param	consumable	The consumable item to consume.
		 */
		void				attack(Fighter& target) noexcept;
		void				defend(void) noexcept;
		bool				flee(Fighter& opponent) noexcept;
		void				receive_damage(Fighter& attacker, unsigned int incoming_damage) noexcept;
		void				apply_status(Status status) noexcept;
		void				consume(Fighter& opponent, Consumable& consumable) noexcept;

		/**
		 * @brief	Heals the fighter for the specified amount.
		 * @param	amount	The healing points to heal.
		 */
		void				heal(unsigned int amount) noexcept;

		// TODO: Work in this docstring
		/**
		 * @brief	Changes the specified stat depending on the provided stage.
		 * @param	stat	The stat to change.
		 * @param	stage	Indicates how much the stat is going to change.
		 */
		void				change_stat(Stat stat, int stage) noexcept;

		/**
		 * @brief	Restores the specified stat. That means that the
		 * 			current specified stat will get to its initial value.
		 * @param	stat	The stat to restore.
		 */
		void				restore_stat(Stat stat) noexcept;

		/**
		 * @brief	Resets all current stats to their original.
		 * 			Mainly used after each battle.
		 * @param	hp	If set to `true`, hp will be reset too.
		 */
		void				reset_stats(bool hp = false) noexcept;

		std::string			status_json(void) const noexcept;
};

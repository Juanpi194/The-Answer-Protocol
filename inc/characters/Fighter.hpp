#pragma once

#include "characters/Character.hpp"
#include "items/Armor.hpp"
#include "items/Weapon.hpp"

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
};

// ? REVIEW: Is this the best way to make this?
enum class FighterType
{
	Player,
	Enemy
};

// Mixin / Abstract class with just pure virtual methods and attributes (not an interface)
class Fighter: public virtual Character
{
	private:
		Armor	*armor;
		Weapon	*weapon;

		static constexpr unsigned int	MIN_HP = 10;
		static constexpr unsigned int	MAX_HP = 100;
		static constexpr unsigned int	MIN_STRENGTH = 5;
		static constexpr unsigned int	MAX_STRENGTH = 80;
		static constexpr unsigned int	MIN_DEFENSE = 3;
		static constexpr unsigned int	MAX_DEFENSE = 90;
		static constexpr unsigned int	MIN_SPEED = 3;
		static constexpr unsigned int	MAX_SPEED = 70;
		// TODO: Add more stats

		/**
		 * @brief	Validates if the provided stats for the fighter are valid.
		 * @note	This method should ONLY be called in the constructor.
		 */
		static bool	validate_stats(t_stats stats);
	protected:
		t_stats				stats;
		Status				status;
		std::list<Item*>	item_list;
	public:
		// Constructors -------------------------------------------------------

		// TODO: Add a way to create a fighter with specific items.

		Fighter(const std::string& name, t_stats stats);
		Fighter(const Fighter& fighter) = delete;
		virtual ~Fighter(void);

		// Operators ----------------------------------------------------------

		Fighter&	operator=(const Fighter& other) = delete;

		// Getters and setters ------------------------------------------------

		Armor					*get_armor(void) const noexcept;
		Weapon					*get_weapon(void) const noexcept;
		t_stats					get_stats(void) const noexcept;
		Status					get_status(void) const noexcept;
		// std::list<Item*>&		get_item_list(void) noexcept;
		const std::list<Item*>&	get_item_list(void) const noexcept;

		void	set_armor(Armor *armor) noexcept;
		void	set_weapon(Weapon *weapon) noexcept;
		void	set_status(Status status) noexcept;

		// Utils --------------------------------------------------------------

		virtual void		choose_action(void) = 0;
		virtual void		attack(Fighter& target) noexcept = 0;
		virtual FighterType	get_type() const noexcept = 0;
		void				apply_status(Status status) noexcept;
};

#pragma once

#include "characters/Character.hpp"
#include "items/Armor.hpp"
#include "items/Weapon.hpp"

enum Status
{
	HEALTHY,
	POISON,
	BURNT,
	FROZEN
};

struct t_stats
{
	unsigned int	hp;
	unsigned int	strength;
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
	protected:
		t_stats				stats;
		Status				status;
		std::list<Item*>	item_list;
	public:
		// Constructors -------------------------------------------------------

		Fighter(const std::string& name, t_stats stats);
		Fighter(const Fighter& fighter);
		virtual ~Fighter(void);

		// Operators ----------------------------------------------------------

		Fighter&	operator=(const Fighter& other) = delete;

		// Getters and setters ------------------------------------------------

		Armor					*get_armor(void) const noexcept;
		Weapon					*get_weapon(void) const noexcept;
		t_stats					get_stats(void) const noexcept;
		Status					get_status(void) const noexcept;
		std::list<Item*>&		get_item_list(void) noexcept;
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

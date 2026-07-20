#pragma once

#include "items/Gear.hpp"

class Fighter;

/**
 * @brief	Abstract class that represents the weapon cathegory of equipment.
 */
class Weapon: public Gear
{
	private:
		/**
		 * @brief	Damage to apply to the on going attack damage.
		 */
		const unsigned int	extra_damage;
		/**
		 * @brief	The register of the total extra damage that this specific sword applied.
		 */
		unsigned int		total_extra_damage;
	public:
		// Constructors -------------------------------------------------------

		Weapon(const std::string& id, const std::string& name, const std::string& description, unsigned int extra_damage, const GearType gear_type);
		Weapon(const Weapon& weapon) = delete;
		virtual ~Weapon(void) = default;

		// Operators ----------------------------------------------------------

		Weapon&	operator=(const Weapon& other) = delete;

		// Getters and setters ------------------------------------------------

		const unsigned int	get_extra_damage(void) const noexcept;
		unsigned int		get_total_extra_damage(void) const noexcept;

		// Utils --------------------------------------------------------------

		// void	use(void) noexcept override;

		virtual void	apply_extra_damage(Fighter& user, Fighter& target, unsigned int initial_damage) noexcept;
};

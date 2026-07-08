#pragma once

#include "items/Gear.hpp"

/**
 * @brief	Abstract class that represents the armor cathegory of equipment.
 */
class Armor: public Gear
{
	private:
		/**
		 * @brief	The damage reduction to apply to the received damage.
		 */
		const unsigned int	damage_reduction;
		/**
		 * @brief	The register of reduced damage by this specific armor.
		 */
		unsigned int		reduced_damage;
	public:
		// Constructors -------------------------------------------------------

		Armor(const std::string& id, const std::string& name, const std::string& description, const unsigned int damage_reduction, const GearType gear_type);
		Armor(const Armor& armor) = delete;
		virtual ~Armor(void);

		// Operators ----------------------------------------------------------

		Armor&	operator=(const Armor& other) = delete;

		// Getters and setters ------------------------------------------------

		const unsigned int	get_damage_reduction(void) const noexcept;
		unsigned int	get_reduced_damage(void) const noexcept;

		// Utils --------------------------------------------------------------

		void	use(void) noexcept override;
};

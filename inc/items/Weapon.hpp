#pragma once

#include "items/Gear.hpp"

class Weapon: public Gear
{
	private:
		const unsigned int	extra_damage;
	public:
		// Constructors -------------------------------------------------------

		Weapon(const std::string& id, const std::string& name, const std::string& description, unsigned int extra_damage);
		Weapon(const Weapon& weapon) = delete;
		~Weapon(void) = default;

		// Operators ----------------------------------------------------------

		Weapon&	operator=(const Weapon& other) = delete;

		// Getters and setters ------------------------------------------------

		unsigned int	get_extra_damage(void) const noexcept;

		// Utils --------------------------------------------------------------

		void	use(void) noexcept override;
};

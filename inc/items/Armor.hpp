#pragma once

#include "items/Gear.hpp"

class Armor: public Gear
{
	private:
		const unsigned int	reduced_damage;
	public:
		// Constructors -------------------------------------------------------

		Armor(const std::string& id, const std::string& name, const std::string& description, unsigned int reduced_damage);
		Armor(const Armor& armor) = delete;
		~Armor(void) = default;

		// Operators ----------------------------------------------------------

		Armor&	operator=(const Armor& other) = delete;

		// Getters and setters ------------------------------------------------

		unsigned int	get_reduced_damage(void) const noexcept;

		// Utils --------------------------------------------------------------

		void	use(void) noexcept override;
};

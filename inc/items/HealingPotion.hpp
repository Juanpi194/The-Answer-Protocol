#pragma once

#include "items/Consumable.hpp"
#include "utils/attributes.hpp"

class HealingPotion final: public Consumable
{
	private:
		static unsigned int				available_id;	// Defined in HealingPotion.cpp
		static const std::string		PREFIX;	// Defined in HealingPotion.cpp
	public:
		static const std::string		NAME;	// Defined in HealingPotion.cpp
		static const std::string		DESCRIPTION;	// Defined in HealingPotion.cpp
		static constexpr unsigned int	HEALING_POINTS = 5;

		// Constructors -------------------------------------------------------

		HealingPotion(void);
		HealingPotion(const HealingPotion& healing_potion);
		~HealingPotion(void) = default;
		HealingPotion	*clone(void) const noexcept override TAP_RETURNS_NONNULL;

		// Operators ----------------------------------------------------------

		HealingPotion&	operator=(const HealingPotion& other) = delete;

		// Getters and setters ------------------------------------------------

		// TODO: Add getters and setters for future attributes.

		// Utils --------------------------------------------------------------

		void	consume(Fighter& user) noexcept override;
};

#pragma once

#include "items/Consumable.hpp"
#include "utils/attributes.hpp"

class FirePotion final: public Consumable
{
	private:
		static unsigned int				available_id;	// Defined in FirePotion.cpp
		static const std::string		PREFIX;	// Defined in FirePotion.cpp
	public:
		static const std::string		NAME;	// Defined in FirePotion.cpp
		static const std::string		DESCRIPTION;	// Defined in FirePotion.cpp
		static constexpr unsigned int	BURN_DAMAGE = 6;

		// Constructors -------------------------------------------------------

		FirePotion(void);
		FirePotion(const FirePotion& fire_potion);
		~FirePotion(void) = default;
		FirePotion	*clone(void) const noexcept override TAP_RETURNS_NONNULL;

		// Operators ----------------------------------------------------------

		FirePotion&	operator=(const FirePotion& other) = delete;

		// Getters and setters ------------------------------------------------

		// TODO: Add getters and setters for future attributes.

		// Utils --------------------------------------------------------------

		void	consume(Fighter& user) noexcept override;
};

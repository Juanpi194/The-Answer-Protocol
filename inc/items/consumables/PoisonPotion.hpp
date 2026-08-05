#pragma once

#include "items/Consumable.hpp"
#include "utils/attributes.hpp"

class PoisonPotion final: public Consumable
{
	private:
		static unsigned int				available_id;	// Defined in PoisonPotion.cpp
		static const std::string		PREFIX;	// Defined in PoisonPotion.cpp
	public:
		static const std::string		NAME;	// Defined in PoisonPotion.cpp
		static const std::string		DESCRIPTION;	// Defined in PoisonPotion.cpp
		static constexpr unsigned int	POISON_POWER = 4;

		// Constructors -------------------------------------------------------

		PoisonPotion(void);
		PoisonPotion(const PoisonPotion& poison_potion);
		~PoisonPotion(void) = default;
		PoisonPotion	*clone(void) const noexcept override TAP_RETURNS_NONNULL;

		// Operators ----------------------------------------------------------

		PoisonPotion&	operator=(const PoisonPotion& other) = delete;

		// Utils --------------------------------------------------------------

		void	consume(Fighter& user) noexcept override;
};

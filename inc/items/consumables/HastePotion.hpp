#pragma once

#include "items/Consumable.hpp"
#include "utils/attributes.hpp"

class HastePotion final: public Consumable
{
	private:
		static unsigned int				available_id;	// Defined in HastePotion.cpp
		static const std::string		PREFIX;	// Defined in HastePotion.cpp
	public:
		static const std::string		NAME;	// Defined in HastePotion.cpp
		static const std::string		DESCRIPTION;	// Defined in HastePotion.cpp
		static constexpr unsigned int	HASTE_DURATION = 3;

		// Constructors -------------------------------------------------------

		HastePotion(void);
		HastePotion(const HastePotion& haste_potion);
		~HastePotion(void) = default;
		HastePotion	*clone(void) const noexcept override TAP_RETURNS_NONNULL;

		// Operators ----------------------------------------------------------

		HastePotion&	operator=(const HastePotion& other) = delete;

		// Getters and setters ------------------------------------------------

		// TODO: Add getters and setters for future attributes.

		// Utils --------------------------------------------------------------

		void	consume(Fighter& user) noexcept override;
};

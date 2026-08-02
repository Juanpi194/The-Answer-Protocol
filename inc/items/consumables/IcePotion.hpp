#pragma once

#include "items/Consumable.hpp"
#include "utils/attributes.hpp"

class IcePotion final: public Consumable
{
	private:
		static unsigned int				available_id;	// Defined in IcePotion.cpp
		static const std::string		PREFIX;	// Defined in IcePotion.cpp
	public:
		static const std::string		NAME;	// Defined in IcePotion.cpp
		static const std::string		DESCRIPTION;	// Defined in IcePotion.cpp
		static constexpr unsigned int	FREEZE_POWER = 4;

		// Constructors -------------------------------------------------------

		IcePotion(void);
		IcePotion(const IcePotion& ice_potion);
		~IcePotion(void) = default;
		IcePotion	*clone(void) const noexcept override TAP_RETURNS_NONNULL;

		// Operators ----------------------------------------------------------

		IcePotion&	operator=(const IcePotion& other) = delete;

		// Getters and setters ------------------------------------------------

		// TODO: Add getters and setters for future attributes.

		// Utils --------------------------------------------------------------

		void	consume(Fighter& user) noexcept override;
};

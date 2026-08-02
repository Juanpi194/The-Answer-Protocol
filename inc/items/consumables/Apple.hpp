#pragma once

#include "items/Consumable.hpp"
#include "utils/attributes.hpp"

class Apple final: public Consumable
{
	private:
		static unsigned int				available_id;	// Defined in Apple.cpp
		static const std::string		PREFIX;	// Defined in Apple.cpp
	public:
		static const std::string		NAME;	// Defined in Apple.cpp
		static const std::string		DESCRIPTION;	// Defined in Apple.cpp
		static constexpr unsigned int	HEALING_POINTS = 5;

		// Constructors -------------------------------------------------------

		Apple(void);
		Apple(const Apple& apple);
		~Apple(void) = default;
		Apple	*clone(void) const noexcept override TAP_RETURNS_NONNULL;

		// Operators ----------------------------------------------------------

		Apple&	operator=(const Apple& other) = delete;

		// Getters and setters ------------------------------------------------

		// TODO: Add getters and setters for future attributes.

		// Utils --------------------------------------------------------------

		void	consume(Fighter& user) noexcept override;
};

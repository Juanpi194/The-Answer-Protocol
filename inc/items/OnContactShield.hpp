#pragma once

class Fighter;

class OnContactShield
{
	public:
		// Constructors -------------------------------------------------------

		virtual	~OnContactShield(void) = default;

		// Operators ----------------------------------------------------------

		OnContactShield&	operator=(const OnContactShield& other) = delete;

		// Utils --------------------------------------------------------------

		virtual void	on_contact(Fighter& user, Fighter& attacker) noexcept = 0;
};

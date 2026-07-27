#pragma once

class Fighter;

class SpecialEffectGear
{
	public:
		// Constructors -------------------------------------------------------

		virtual ~SpecialEffectGear(void) = default;

		// Operators ----------------------------------------------------------

		SpecialEffectGear&	operator=(const SpecialEffectGear& other) = delete;

		// Utils --------------------------------------------------------------

		virtual void	special_effect(Fighter& user, Fighter& opponent) noexcept = 0;
};

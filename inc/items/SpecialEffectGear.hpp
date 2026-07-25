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

		void	special_effect(Fighter& user, Fighter& affected);
};

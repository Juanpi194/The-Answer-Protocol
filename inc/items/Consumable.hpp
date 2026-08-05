#pragma once

#include "items/Item.hpp"

class Fighter;

class Consumable: public Item
{
	public:
		// Constructors -------------------------------------------------------

		Consumable(const std::string& id, const std::string& name, const std::string& description);
		Consumable(const Consumable& consumable) = delete;
		virtual ~Consumable(void) = default;

		// Operators ----------------------------------------------------------

		Consumable&	operator=(const Consumable& other) = delete;

		// Utils --------------------------------------------------------------

		virtual void	consume(Fighter& user) noexcept = 0;
};

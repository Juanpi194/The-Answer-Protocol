#pragma once

#include "items/Item.hpp"

class Consumable: public Item
{
	private:
		// TODO: Add some attributes
	public:
		// Constructors -------------------------------------------------------

		Consumable(const std::string& name, const std::string& description);
		Consumable(const Consumable& consumable);
		virtual ~Consumable(void) = default;

		// Operators ----------------------------------------------------------

		Consumable&	operator=(const Consumable& other) = delete;

		// Getters and setters ------------------------------------------------

		// TODO: Getters and setters

		// Utils --------------------------------------------------------------

		void	use(void) noexcept override;
};

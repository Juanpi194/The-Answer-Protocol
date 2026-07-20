#pragma once

#include "items/Item.hpp"

class Fighter;

/**
 * @brief	Abstract class that represents the shield category of items.
 */
class Shield: public Item
{
	private:
		/**
		 * @brief	Counter of how many times the item has been used.
		 */
		unsigned int	uses;
		const unsigned int	max_uses;
	public:
		// Constructors -------------------------------------------------------

		Shield(const std::string& id, const std::string& name, const std::string& description, const unsigned int max_uses);
		Shield(const Shield& shield) = delete;
		virtual ~Shield(void) = default;

		// Operators ----------------------------------------------------------

		Shield&	operator=(const Shield& other) = delete;

		// Getters and setters ------------------------------------------------

		unsigned int		get_uses(void) const noexcept;
		const unsigned int	get_max_uses(void) const noexcept;

		void	set_uses(unsigned int uses) noexcept;

		// Utils --------------------------------------------------------------

		// void	use(void) noexcept override;

		virtual void	protect(Fighter& user, Fighter& attacker) noexcept;
};

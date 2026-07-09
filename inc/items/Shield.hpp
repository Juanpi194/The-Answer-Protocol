#pragma once

#include "items/Item.hpp"

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

		Shield(const std::string& name, const std::string& description, const unsigned int max_uses);
		/**
		 * @note	The copy will set `uses` to 0.
		 */
		Shield(const Shield& shield);
		virtual ~Shield(void) = default;

		// Operators ----------------------------------------------------------

		Shield&	operator=(const Shield& other) = delete;

		// Getters and setters ------------------------------------------------

		unsigned int		get_uses(void) const noexcept;
		const unsigned int	get_max_uses(void) const noexcept;

		void	set_uses(unsigned int uses) noexcept;

		// Utils --------------------------------------------------------------

		void	protect(void) noexcept;
};

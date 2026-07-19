#pragma once
#include <string>

class Player;

/**
 * @brief	Interface for vendor characters.
 */
class Vendor
{
	public:
		// Constructors -------------------------------------------------------

		virtual ~Vendor(void) = default;

		// Operators ----------------------------------------------------------

		Vendor&	operator=(const Vendor& other) = delete;

		// Utils --------------------------------------------------------------

		virtual void	on_buy(Player& player, const std::string& product) noexcept = 0;
};

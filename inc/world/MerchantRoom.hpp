#pragma once

#include "world/Room.hpp"

class Merchant;

class MerchantRoom final: public Room
{
	public:
		// Constructors -------------------------------------------------------
		MerchantRoom(const std::string& id, const std::string& name, const std::string description, Merchant* merchant);
		MerchantRoom(const MerchantRoom& merchant_room) = delete;
		~MerchantRoom(void);

		// Operators ----------------------------------------------------------

		MerchantRoom&	operator=(const MerchantRoom& other) = delete;

		// Getters and setters ------------------------------------------------

		// TODO

		// Utils --------------------------------------------------------------

		void	on_enter(void) noexcept override;
};

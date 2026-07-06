#pragma once

#include "Room.hpp"

class MerchantRoom final: public Room
{
	public:
		// Constructors
		MerchantRoom(const std::string& id, const std::string& name, const std::string description, Merchant* merchant);
		MerchantRoom(const MerchantRoom& safe_room) = delete;
		~MerchantRoom(void);

		// Operators
		MerchantRoom&	operator=(const MerchantRoom& other) = delete;

		// Getters and setters
		// TODO

		// Utils
		void	complete_zone(void) override;
};

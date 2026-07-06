#pragma once

#include "Room.hpp"

/*
No npcs, heals the players (for example)
*/
class SafeRoom final: public Room 
{
	public:
		// Constructors
		SafeRoom(const std::string& id, const std::string& name, const std::string description);
		SafeRoom(const SafeRoom& safe_room) = delete;
		~SafeRoom(void);

		// Operators
		SafeRoom&	operator=(const SafeRoom& other) = delete;

		// Getters and setters
		// TODO

		// Utils
		void	complete_zone(void) override;
};

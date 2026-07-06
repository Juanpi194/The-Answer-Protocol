#pragma once

#include "Room.hpp"
#include "Enemy.hpp"

class EnemyRoom final: public Room
{
	public:
		// Constructors
		EnemyRoom(const std::string& id, const std::string& name, const std::string description, Enemy *enemy);
		EnemyRoom(const EnemyRoom& safe_room) = delete;
		~EnemyRoom(void);

		// Operators
		EnemyRoom&	operator=(const EnemyRoom& other) = delete;

		// Getters and setters
		// TODO

		// Utils
		void	complete_zone(void) override;
};

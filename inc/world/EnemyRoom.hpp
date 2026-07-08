#pragma once

#include "world/Room.hpp"

class Enemy;

class EnemyRoom final: public Room
{
	public:
		// Constructors -------------------------------------------------------

		EnemyRoom(const std::string& id, const std::string& name, const std::string description, Enemy *enemy);
		EnemyRoom(const EnemyRoom& enemy_room) = delete;
		~EnemyRoom(void);

		// Operators ----------------------------------------------------------

		EnemyRoom&	operator=(const EnemyRoom& other) = delete;

		// Getters and setters ------------------------------------------------

		// TODO

		// Utils --------------------------------------------------------------

		void	on_enter(void) noexcept override;
};

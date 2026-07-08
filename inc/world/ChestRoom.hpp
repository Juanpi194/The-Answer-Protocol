#pragma once

#include "world/Room.hpp"
#include "items/Chest.hpp"
// TODO: Finish this class

class Enemy;

class ChestRoom final: public Room
{
	private:
		// TODO: Add chest.
		Chest	chest;
	public:
		// Constructors -------------------------------------------------------

		ChestRoom(const std::string& name, const std::string& description, Enemy *enemy);
		ChestRoom(const ChestRoom& chest_room) = delete;
		~ChestRoom(void);

		// Operators ----------------------------------------------------------

		ChestRoom&	operator=(const ChestRoom& other) = delete;

		// Getters and setters ------------------------------------------------

		const Chest&	get_chest(void) const noexcept;

		// Utils --------------------------------------------------------------

		void	on_enter(void) noexcept override;

};

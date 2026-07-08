#pragma once

#include "world/Room.hpp"

// TODO: Finish this class

class ChestRoom final: public Room
{
	public:
		// Constructors -------------------------------------------------------

		ChestRoom(const std::string& name, const std::string& description);
		ChestRoom(const ChestRoom& chest_room) = delete;
		~ChestRoom(void);

		// Operators ----------------------------------------------------------

		ChestRoom&	operator=(const ChestRoom& other) = delete;

		// Getters and setters ------------------------------------------------

		// TODO

		// Utils --------------------------------------------------------------

		void	on_enter(void) noexcept override;

};

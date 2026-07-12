#pragma once
#include <string>
#include <list>

class Item;
class Room;
class Player;

class Character
{
	private:
		std::string			name;
	protected:
		Room				*current_room;
	public:
		// Constructors -------------------------------------------------------

		explicit Character(const std::string& name);
		Character(const Character& character) = delete;
		virtual ~Character(void) = default;

		// Operators ----------------------------------------------------------

		Character&	operator=(const Character& other) = delete;

		// Getters and setters ------------------------------------------------

		std::string				get_name(void) const noexcept;
		Room					*get_current_room(void) const noexcept;

		void	set_name(const std::string& name);
		void	set_current_room(Room *room) noexcept;

		// Utils --------------------------------------------------------------

		virtual void	on_interact(Player& player) = 0;
};

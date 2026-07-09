#include "characters/Character.hpp"

#include <stdexcept>

#include "utils/utils.hpp"
#include "world/Room.hpp"

// Constructors ---------------------------------------------------------------

Character::Character(const std::string& name):
	name(name),
	current_room(nullptr)
{
	std::string	temp = name;

	trim_str(temp);
	if (temp.empty())
		throw std::invalid_argument("Character name cannot be empty");
}

Character::Character(const Character& character):
	name(character.name),
	current_room(character.current_room)
{
	log("Character '" + character.name + "' was copied", LogLevel::DEBUG);
}

// Operators ------------------------------------------------------------------

Character&	Character::operator=(const Character& other)
{
	if (this == &other)
		return (*this);
	name = other.name;
	current_room = other.current_room;
	return (*this);
}

// Getters and setters --------------------------------------------------------

std::string				Character::get_name(void) const noexcept
{
	return (name);
}

Room					*Character::get_current_room(void) const noexcept
{
	return (current_room);
}

void	Character::set_name(const std::string& name)
{
	std::string	temp = name;

	trim_str(temp);
	if (temp.empty())
	{
		log("Tried to set empty name. Change was not done.", LogLevel::WARNING);
		return ;
	}
	this->name = name;
}

// Utils ----------------------------------------------------------------------


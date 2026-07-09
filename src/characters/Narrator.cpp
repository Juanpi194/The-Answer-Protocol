#include "characters/Narrator.hpp"

// Constructors ---------------------------------------------------------------

Narrator::Narrator(const std::string& name, const std::string& description, const std::string& sentence):
	Character(name),	
	NPC(name, description),
	sentence(sentence)	
{
	// ? REVIEW: Check sentence logic
}

// Getters and setters --------------------------------------------------------

std::string	Narrator::get_sentence(void) const noexcept
{
	return (sentence);
}

// Utils ----------------------------------------------------------------------

void	Narrator::on_interact(Player& player)
{
	// TODO: Logic...
}

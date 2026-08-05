#include "characters/Narrator.hpp"

#include "characters/Player.hpp"

unsigned int		Narrator::available_id = 0;
const std::string	Narrator::PREFIX = "narrator.";

// Constructors ---------------------------------------------------------------

Narrator::Narrator(const std::string& name, const std::string& description, const std::string& sentence):
	Character(name),	
	NPC(NPC::PREFIX + PREFIX + std::to_string(available_id++), name, description),
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

const std::string	Narrator::on_talk(Player& player) noexcept
{
	return (sentence);
}

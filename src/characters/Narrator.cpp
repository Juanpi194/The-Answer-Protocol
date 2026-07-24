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

void	Narrator::on_talk(Player& player) noexcept
{
	// ? REVIEW: Is this everything?
	player.send_to_outbox(sentence);
}

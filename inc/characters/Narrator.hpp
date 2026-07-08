#pragma once

#include "characters/NPC.hpp"

class Narrator final: public NPC
{
	private:
		const std::string	sentence;
	public:
		// Constructors -------------------------------------------------------

		Narrator(const std::string& id, const std::string& name, const std::string& description, const std::string& sentence);
		Narrator(const Narrator& narrator) = delete;
		~Narrator(void) = default;

		// Operators ----------------------------------------------------------

		Narrator&	operator=(const Narrator& other) = delete;

		// Getters and setters ------------------------------------------------

		std::string	get_sentence(void) const noexcept;

		// Utils --------------------------------------------------------------

		void	on_interact(Player& player) override;
};

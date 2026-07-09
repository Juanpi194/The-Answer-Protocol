#pragma once

#include "characters/NPC.hpp"

class Narrator final: public NPC
{
	private:
		const std::string	sentence;
	public:
		// Constructors -------------------------------------------------------

		// ! FIXME: Change sentences logic
		Narrator(const std::string& name, const std::string& description, const std::string& sentence);

		/**
		 * @note	It is not allowed to make a copy because each narrator should be unique.
		 */
		Narrator(const Narrator& narrator) = delete;
		~Narrator(void) = default;

		// Operators ----------------------------------------------------------

		Narrator&	operator=(const Narrator& other) = delete;

		// Getters and setters ------------------------------------------------

		std::string	get_sentence(void) const noexcept;

		// Utils --------------------------------------------------------------

		void	on_interact(Player& player) override;
};

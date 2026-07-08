#pragma once

#include "characters/Character.hpp"

class NPC: public virtual Character
{
	private:
		const std::string	id;
		const std::string	description;

		static const std::string	PREFIX;	// Defined in NPC.cpp
		static constexpr bool		TITLE_NAME = true;
		static constexpr size_t		MIN_NAME_LENGTH = 3;
		static constexpr size_t		MAX_NAME_LENGTH = 18;
		static constexpr size_t		MIN_DESCRIPTION_LENGTH = 8;
		static constexpr size_t		MAX_DESCRIPTION_LENGTH = 60;

		/**
		 * @brief	Verifies that the provided arguments follow the specified
		 * 			rules.
		 * @returns	`true` if all parameters follow the expected. `false` otherwise.
		 * @note	This method should ONLY be used in the constructor.
		 */
		static bool	validate_arguments(const std::string& id, const std::string& name, const std::string& description);
	public:
		// Constructors -------------------------------------------------------

		NPC(const std::string& id, const std::string& name, const std::string& description);
		NPC(const NPC& npc) = delete;
		virtual ~NPC(void) = default;

		// Operators ----------------------------------------------------------

		NPC&	operator=(const NPC& other) = delete;

		// Getters and setters ------------------------------------------------

		std::string	get_id(void) const noexcept;
		std::string	get_description(void) const noexcept;

		// Utils --------------------------------------------------------------

		// TODO: Util functions
};

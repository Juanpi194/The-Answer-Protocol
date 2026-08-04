#pragma once

#include "characters/Enemy.hpp"

class Ogre final: public Enemy
{
	private:
		static unsigned int			available_id;	// Defined in Ogre.cpp
		static const std::string	PREFIX;	// Definied in Ogre.cpp
	public:
		static const std::string	NAME;	// Defined in Ogre.cpp
		static const std::string	DESCRIPTION;	// Defined in Ogre.cpp
		static const t_stats		DEFAULT_STATS;	// Defined in Ogre.cpp
		static const unsigned int	DEFAULT_GOLD = 35;

		// Constructors -------------------------------------------------------

		Ogre(void);
		Ogre(const Ogre& ogre);
		~Ogre(void) = default;
		Ogre	*clone(void) const noexcept override TAP_RETURNS_NONNULL;

		// Operators ----------------------------------------------------------

		Ogre&	operator=(const Ogre& other) = delete;

		// Getters and setters ------------------------------------------------

		// TODO: Add specific attributes to the ogre and their getters and setters

		// Utils --------------------------------------------------------------

		FightChoice	choose_action(void) const noexcept override;
};

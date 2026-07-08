#pragma once

#include "characters/NPC.hpp"

class Gear;

class Enchanter final: public NPC
{
	private:
		// std::map<Enchantments, unsigned int>	enchantments;	// Enchantment and price
	public:
		// Constructors -------------------------------------------------------

		// TODO: Add enchantments to the constructor
		Enchanter(const std::string& name, const std::string& description);
		Enchanter(const Enchanter& enchanter) = delete;
		~Enchanter(void);

		// Operators ----------------------------------------------------------

		Enchanter&	operator=(const Enchanter& other) = delete;

		// Getters and setters ------------------------------------------------

		// TODO: Add the map

		// Utils --------------------------------------------------------------
		// TODO: Add enchantment to this function. Throws exception if enchantment doesn't work for gear.
		void	on_interact(Player& player) override;
		void	enchant(Gear *gear);
};

#pragma once
#include <list>
#include <map>

class Item;
class Player;

class Chest
{
	private:
		bool	opened;
		// TODO: Add item pool.
		static const std::map<Item*, unsigned int>	POSSIBLE_ITEMS;	// Defined in Chest.cpp

		/**
		 * @brief	Opens the chest, generating random items from the pool.
		 * 			Sets `opened` to `true`.
		 * @returns	A list with all the generated items. 
		 */
		std::list<Item*>	open(void) noexcept;
	public:
		// Constructors -------------------------------------------------------

		Chest(void);
		Chest(const Chest& chest) = delete;
		~Chest(void) = default;

		// Operators ----------------------------------------------------------

		Chest&	operator=(const Chest& other) = delete;

		// Getters and setters ------------------------------------------------

		// TODO: Ni idea

		// Utils --------------------------------------------------------------

		/**
		 * @brief	Checks if the player has a key. If so,
		 * 			the `open` method will be used,
		 * 			generating a list of random items.
		 * @param	player	The player that interacted with the chest.
		 * @returns	The list of the items if the player had the key,
		 * 			`nullptr` otherwise.
		 */
		std::list<Item*>	*interact(Player& player) noexcept;
};

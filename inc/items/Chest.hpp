#pragma once
#include <list>
#include <map>

#include "utils/attributes.hpp"

class Item;
class Player;
class Enemy;

class Chest
{
	private:
		bool							opened;
		std::map<Item*, unsigned int>	pool;
		Enemy							*guardian;

		/**
		 * @brief	Opens the chest, generating random items from the pool.
		 * 			Sets `opened` to `true`.
		 * @returns	A list with all the generated items. An empty list if
		 * 			the chest is already opened.
		 * @note	If the chest is not opened, this method will be in a
		 * 			loop until the result list size is not 0.
		 * 			That means that it will always generate at least
		 * 			one item.
		 */
		std::list<Item*>	open(void) noexcept TAP_COLD TAP_UNUSED_RESULT;

		/**
		 * @brief	Checks if the player has beaten the guardian of the chest.
		 */
		bool				player_beat_guardian(Player& player) noexcept TAP_UNUSED_RESULT;
	public:
		// Constructors -------------------------------------------------------

		Chest(void);
		Chest(Enemy *guardian);
		Chest(const Chest& chest) = delete;
		~Chest(void);

		// Operators ----------------------------------------------------------

		Chest&	operator=(const Chest& other) = delete;

		// Getters and setters ------------------------------------------------

		bool									is_opened(void) const noexcept;
		const std::map<Item*, unsigned int>&	get_pool(void) const noexcept;
		Enemy									*get_guardian(void) const noexcept;

		// Utils --------------------------------------------------------------

		/**
		 * @brief	Checks if the player has a key, and, in case there is
		 * 			an enemy room, checks if the enemy is an already beaten
		 * 			enemy by the player. If so, the `open` method will be used,
		 * 			generating a list of random items and consuming a key.
		 * @param	player	The player that interacted with the chest.
		 * @returns	If the chest is already opened, an empty list.
		 * 			If not, the list of the items if the player had a key
		 * 			and if the guardian was beaten (if there was one),
		 * 			an empty list otherwise.
		 */
		std::list<Item*>	interact(Player& player) noexcept TAP_UNUSED_RESULT;
};

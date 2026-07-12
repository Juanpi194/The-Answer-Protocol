#pragma once
#include <list>
#include <map>

class Item;
class Player;

class Chest
{
	private:
		bool							opened;
		std::map<Item*, unsigned int>	pool;

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
		std::list<Item*>	open(void) noexcept __COLD __attribute_warn_unused_result__;
	public:
		// Constructors -------------------------------------------------------

		Chest(void);
		Chest(const Chest& chest) = delete;
		~Chest(void);

		// Operators ----------------------------------------------------------

		Chest&	operator=(const Chest& other) = delete;

		// Getters and setters ------------------------------------------------

		bool									is_opened(void) const noexcept;
		std::map<Item*, unsigned int>&			get_pool(void) noexcept;
		const std::map<Item*, unsigned int>&	get_pool(void) const noexcept;

		// Utils --------------------------------------------------------------

		/**
		 * @brief	Checks if the player has a key. If so,
		 * 			the `open` method will be used,
		 * 			generating a list of random items.
		 * @param	player	The player that interacted with the chest.
		 * @returns	If the chest is already opened, an empty list.
		 * 			If not, the list of the items if the player had a key,
		 * 			an empty list otherwise.
		 */
		std::list<Item*>	interact(Player& player) noexcept __attribute_warn_unused_result__;
};

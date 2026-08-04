#pragma once

#include "characters/NPC.hpp"
#include "characters/Fighter.hpp"

class Enemy: public NPC, public Fighter
{
	private:
		unsigned int	gold;	// Will be dropped when defeated
	public:
		// Constructors -------------------------------------------------------

		Enemy(const std::string& id, const std::string& name, const std::string& description, t_stats stats, unsigned int gold);
		Enemy(const Enemy& enemy) = delete;
		virtual	~Enemy(void) = default;
		virtual Enemy	*clone(void) const noexcept TAP_RETURNS_NONNULL = 0;

		// Operators ----------------------------------------------------------

		Enemy&	operator=(const Enemy& other) = delete;

		// Getters and setters ------------------------------------------------

		unsigned int	get_gold(void) const noexcept;

		// Utils --------------------------------------------------------------

		const std::string	on_talk(Player& player) noexcept override;

		/**
		 * @brief	Each enemy type chooses which action to perform their own
		 * 			way, but will never be `FLEE`.
		 * @returns	The choice, with the action to perform and a possible item
		 * 			to consume.
		 */
		virtual FightChoice	choose_action(void) const noexcept = 0;

};

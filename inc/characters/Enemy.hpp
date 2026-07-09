#pragma once

#include "characters/NPC.hpp"
#include "characters/Fighter.hpp"

class Enemy: public NPC, public Fighter
{
	private:
		unsigned int	gold;	// Will be dropped when defeated
	public:
		// Constructors -------------------------------------------------------

		Enemy(const std::string& name, const std::string& description, t_stats stats, unsigned int gold);
		Enemy(const Enemy& enemy);
		virtual	~Enemy(void);

		// Operators ----------------------------------------------------------

		Enemy&	operator=(const Enemy& other) = delete;

		// Getters and setters ------------------------------------------------

		unsigned int	get_gold(void) const noexcept;

		// Utils --------------------------------------------------------------

		void			on_interact(Player& player) override;
		FighterType		get_type(void) const noexcept override;

		/**
		 * @deprecated	No default behavior for enemy, each enemy MUST
		 * 				have its own way of attacking.
		 * @brief	Default behavior for attacking. Some enemies will override
		 * 			this method to have their unique way of attacking.
		 * @param	target	Pointer to the target that will be attacked.
		 */
		// virtual void	attack(Fighter *target) noexcept override;
};

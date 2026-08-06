#include "characters/enemies/Shade.hpp"

#include "factories/ItemFactory.hpp"
#include "items/weapons/SerratedSword.hpp"

unsigned int		Shade::available_id = 0;
const std::string	Shade::PREFIX = "shade.";
const std::string	Shade::NAME = "Shade";
const std::string	Shade::DESCRIPTION = "An elusive, lurking shadow.";
const t_stats		Shade::DEFAULT_STATS = {2, 10, 3, 1, 4, 10, 3, 1, 4};

// Constructors ---------------------------------------------------------------

Shade::Shade(void):
	Character(NAME),
	Enemy(NPC::PREFIX + PREFIX + std::to_string(available_id++), NAME, DESCRIPTION, DEFAULT_STATS, DEFAULT_GOLD)
{
	set_weapon(ItemFactory::create_serrated_sword());
}

Shade::Shade(const Shade& shade):
	Character(NAME),
	Enemy(NPC::PREFIX + PREFIX + std::to_string(available_id++), NAME, DESCRIPTION, DEFAULT_STATS, DEFAULT_GOLD)
{
}

Shade	*Shade::clone(void) const noexcept
{
	return (new Shade());
}

// Utils ----------------------------------------------------------------------

FightChoice	Shade::choose_action(void) const noexcept
{
	int	roll;

	roll = rand() % 3;
	if (roll == 0)
		return (FightChoice{FightAction::DEFEND});
	return (FightChoice{FightAction::ATTACK});
}

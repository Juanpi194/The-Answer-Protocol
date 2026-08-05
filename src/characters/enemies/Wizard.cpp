#include "characters/enemies/Wizard.hpp"

#include "factories/ItemFactory.hpp"
#include "items/armor/FrostArmor.hpp"
#include "items/weapons/FrostSword.hpp"
#include "items/shields/SteelShield.hpp"

unsigned int		Wizard::available_id = 0;
const std::string	Wizard::PREFIX = "wizard.";
const std::string	Wizard::NAME = "Wizard";
const std::string	Wizard::DESCRIPTION = "A skilled elemental spellcaster.";
const t_stats		Wizard::DEFAULT_STATS = {3, 20, 5, 3, 3, 20, 5, 3, 3};

// Constructors ---------------------------------------------------------------

Wizard::Wizard(void):
	Character(NAME),
	Enemy(NPC::PREFIX + PREFIX + std::to_string(available_id++), NAME, DESCRIPTION, DEFAULT_STATS, DEFAULT_GOLD)
{
	set_armor(ItemFactory::create_frost_armor());
	set_shield(ItemFactory::create_steel_shield());
	set_weapon(ItemFactory::create_frost_sword());
}

Wizard::Wizard(const Wizard& wizard):
	Character(NAME),
	Enemy(NPC::PREFIX + PREFIX + std::to_string(available_id++), NAME, DESCRIPTION, DEFAULT_STATS, DEFAULT_GOLD)
{
}

Wizard	*Wizard::clone(void) const noexcept
{
	return (new Wizard());
}

// Utils ----------------------------------------------------------------------

FightChoice	Wizard::choose_action(void) const noexcept
{
	int	roll;

	roll = rand() % 3;
	if (roll == 0)
		return (FightChoice{FightAction::DEFEND});
	return (FightChoice{FightAction::ATTACK});
}

#include "characters/enemies/Ogre.hpp"

#include "factories/ItemFactory.hpp"
#include "items/armor/BronzeArmor.hpp"
#include "items/shields/BronzeShield.hpp"
#include "items/weapons/BronzeSword.hpp"
#include "items/consumables/FirePotion.hpp"
#include "items/consumables/PoisonPotion.hpp"

unsigned int		Ogre::available_id = 0;
const std::string	Ogre::PREFIX = "ogre.";
const std::string	Ogre::NAME = "Ogre";
const std::string	Ogre::DESCRIPTION = "A huge and brutal beast.";
const t_stats		Ogre::DEFAULT_STATS = {2, 15, 4, 3, 1, 15, 4, 3, 1};

// Constructors ---------------------------------------------------------------

Ogre::Ogre(void):
	Character(NAME),
	Enemy(NPC::PREFIX + PREFIX + std::to_string(available_id++), NAME, DESCRIPTION, DEFAULT_STATS, DEFAULT_GOLD)
{
	set_armor(ItemFactory::create_bronze_armor());
	set_shield(ItemFactory::create_bronze_shield());
	set_weapon(ItemFactory::create_bronze_sword());
	get_inventory().add_item(ItemFactory::create_fire_potion());
	get_inventory().add_item(ItemFactory::create_poison_potion());
}

Ogre::Ogre(const Ogre& ogre):
	Character(NAME),
	Enemy(NPC::PREFIX + PREFIX + std::to_string(available_id++), NAME, DESCRIPTION, DEFAULT_STATS, DEFAULT_GOLD)
{
	(void)ogre;
}

Ogre	*Ogre::clone(void) const noexcept
{
	return (new Ogre());
}

// Utils ----------------------------------------------------------------------

FightChoice	Ogre::choose_action(void) noexcept
{
	int			roll;
	Consumable	*consumable;

	roll = rand() % 3;
	consumable = roll_consumable(CONSUME_CHANCE);
	if (consumable)
		return (FightChoice{FightAction::CONSUME, consumable});
	if (roll == 0)
		return (FightChoice{FightAction::DEFEND});
	return (FightChoice{FightAction::ATTACK});
}

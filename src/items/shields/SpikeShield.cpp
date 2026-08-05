#include "items/shields/SpikeShield.hpp"

#include "characters/Fighter.hpp"

unsigned int		SpikeShield::available_id = 0;
const std::string	SpikeShield::PREFIX = "spike_shield.";
const std::string	SpikeShield::NAME = "Spike Shield";
const std::string	SpikeShield::DESCRIPTION = "Blocks attacks, deals damage when receiving an attack, and applies poison to the attacker.";

// Constructors ---------------------------------------------------------------

SpikeShield::SpikeShield(void):
	Shield(Item::PREFIX + PREFIX + std::to_string(available_id++), NAME, DESCRIPTION, MAX_USES)
{
}

SpikeShield::SpikeShield(const SpikeShield& spike_shield):
	Shield(Item::PREFIX + PREFIX + std::to_string(available_id++), spike_shield.get_name(), spike_shield.get_description(), spike_shield.get_max_uses())
{
}

SpikeShield	*SpikeShield::clone(void) const noexcept
{
	return (new SpikeShield(*this));
}

// Utils ----------------------------------------------------------------------

void	SpikeShield::on_contact(Fighter& user, Fighter& attacker) noexcept
{
	attacker.lose_hp(REFLECT_DAMAGE);
	attacker.apply_status(IN_CONTACT_STATUS);
}

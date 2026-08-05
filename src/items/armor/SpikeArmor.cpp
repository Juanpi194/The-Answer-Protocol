#include "items/armor/SpikeArmor.hpp"


unsigned int		SpikeArmor::available_id = 0;
const std::string	SpikeArmor::PREFIX = "spike_armor.";
const std::string	SpikeArmor::NAME = "Spike Armor";
const std::string	SpikeArmor::DESCRIPTION = "Reduces the incoming damage by a bit, then inflicts some damage back.";

// Constructors ---------------------------------------------------------------

SpikeArmor::SpikeArmor(void):
	Armor(Item::PREFIX + PREFIX + std::to_string(available_id++), NAME, DESCRIPTION, DAMAGE_REDUCTION, GEAR_TYPE)
{
}

SpikeArmor::SpikeArmor(const SpikeArmor& spike_armor):
	Armor(Item::PREFIX + PREFIX + std::to_string(available_id++), spike_armor.get_name(), spike_armor.get_description(), spike_armor.get_damage_reduction(), spike_armor.get_gear_type())
{
}

SpikeArmor	*SpikeArmor::clone(void) const noexcept
{
	return (new SpikeArmor(*this));
}

// Utils ----------------------------------------------------------------------

void	SpikeArmor::special_effect(Fighter& user, Fighter& opponent) noexcept
{
	opponent.lose_hp(REFLECT_DAMAGE);
	opponent.apply_status(APPLIED_STATUS);
}

#include "items/ChestKey.hpp"

unsigned int		ChestKey::available_id = 0;
const std::string	ChestKey::PREFIX = "chest_key.";
const std::string	ChestKey::NAME = "Chest Key";
const std::string	ChestKey::DESCRIPTION = "Opens a chest.";

// Constructors ---------------------------------------------------------------

ChestKey::ChestKey(void):
	Item(Item::PREFIX + PREFIX + std::to_string(available_id++), NAME, DESCRIPTION)
{
}

ChestKey::ChestKey(const ChestKey& chest_key):
	Item(Item::PREFIX + PREFIX + std::to_string(available_id++), chest_key.get_name(), chest_key.get_description())
{
}

ChestKey	*ChestKey::clone(void) const noexcept
{
	return (new ChestKey(*this));
}

// Utils ----------------------------------------------------------------------

// void	ChestKey::use(void) noexcept
// {
// 	// TODO: Logic...
// }

#include "items/IronShield.hpp"

// Constructors ---------------------------------------------------------------

IronShield::IronShield(const std::string& id, const std::string& name, const std::string& description):
	Shield(id, name, description, MAX_USES)
{
}

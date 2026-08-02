#pragma once
#include <string>

#include "utils/attributes.hpp"

class NPC;
class Enchanter;

class NpcPacificFactory
{
	public:
		static 	NPC		*create_from_name(const std::string& npc_name) TAP_RETURNS_NONNULL TAP_UNUSED_RESULT;

		// Specific enemies ---------------------------------------------------

		static Enchanter	*create_enchanter(void) noexcept TAP_RETURNS_NONNULL TAP_UNUSED_RESULT;
};
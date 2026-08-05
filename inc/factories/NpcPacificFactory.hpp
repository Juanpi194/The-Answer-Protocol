#pragma once
#include <string>

#include "utils/attributes.hpp"
#include "characters/QuestGiver.hpp"

class NPC;
class Enchanter;
class Merchant;
class Narrator;

class NpcPacificFactory
{
	public:
		// Specific npcs ------------------------------------------------------

		// Enchanter --

		static Enchanter	*create_enchanter_basic(const std::string& name, const std::string& description) TAP_RETURNS_NONNULL TAP_UNUSED_RESULT;
		static Enchanter	*create_enchanter_advanced(const std::string& name, const std::string& description) TAP_RETURNS_NONNULL TAP_UNUSED_RESULT;

		// Merchant --

		static Merchant		*create_merchant_basic(const std::string& name, const std::string& description) TAP_RETURNS_NONNULL TAP_UNUSED_RESULT;
		static Merchant		*create_merchant_advanced(const std::string& name, const std::string& description) TAP_RETURNS_NONNULL TAP_UNUSED_RESULT;

		// Narrator --

		static Narrator		*create_narrator(const std::string& name, const std::string& description, const std::string& sentence) TAP_RETURNS_NONNULL TAP_UNUSED_RESULT;

		// Quest giver --

		static QuestGiver	*create_quest_giver(const std::string& name, const std::string& description, Quest& quest, t_quest_dialogues dialogues) TAP_RETURNS_NONNULL TAP_UNUSED_RESULT;
};

#pragma once
#include <string>

#include "utils/attributes.hpp"

class Enemy;
class Goblin;
class Ogre;
class Shade;
class Wizard;
class Shadow;
class Dragon;

class EnemyFactory
{
	public:
		static Enemy	*create_from_name(const std::string& enemy_name) TAP_RETURNS_NONNULL TAP_UNUSED_RESULT;

		// Specific enemies ---------------------------------------------------

		static Goblin	*create_goblin(void) TAP_RETURNS_NONNULL TAP_UNUSED_RESULT;
		static Ogre		*create_ogre(void) TAP_RETURNS_NONNULL TAP_UNUSED_RESULT;
		static Shade	*create_shade(void) TAP_RETURNS_NONNULL TAP_UNUSED_RESULT;
		static Wizard	*create_wizard(void) TAP_RETURNS_NONNULL TAP_UNUSED_RESULT;
		static Shadow	*create_shadow(void) TAP_RETURNS_NONNULL TAP_UNUSED_RESULT;
		static Dragon	*create_dragon(void) TAP_RETURNS_NONNULL TAP_UNUSED_RESULT;
};
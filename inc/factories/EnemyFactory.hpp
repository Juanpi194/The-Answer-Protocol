#pragma once
#include <string>

#include "utils/attributes.hpp"

class Enemy;
class Goblin;

class EnemyFactory
{
	public:
		static Enemy	*create_from_name(const std::string& enemy_name) TAP_RETURNS_NONNULL TAP_UNUSED_RESULT;

		// Specific enemies ---------------------------------------------------

		static Goblin	*create_goblin(void) noexcept TAP_RETURNS_NONNULL TAP_UNUSED_RESULT;
};
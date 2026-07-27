#include "commands/CommandHandler.hpp"

#include "utils/utils.hpp"
#include "battle/Battle.hpp"
#include "characters/Player.hpp"
#include "characters/Enemy.hpp"
#include "world/Room.hpp"
#include "world/World.hpp"

/**
 * @brief	Checks if there is an enemy in the specified room.
 * @param	room	The room.
 * @returns	`nullptr` if there is no enemy in the room. A pointer to
 * 			the enemy otherwise.
 */
static Enemy	*get_enemy_in_room(const Room& room)
{
	NPC		*npc;
	Enemy	*enemy;

	npc = room.get_NPC();
	if (!npc)
		return (nullptr);
	enemy = dynamic_cast<Enemy*>(npc);
	if (!enemy)
		return (nullptr);
	return (enemy);
}

/**
 * @brief	Used by `cmd_attack_defend_flee` function. Cleans
 * 			everything related to the player with the fight.
 */
static void		fight_result(Player& player, World& world)
{
	player.reset_stats();
	if (player.get_battle()->get_winner() == &player)
	{
		// TODO: Reward player for winning, add the enemy to the beaten enemies, ...
	}
	delete (player.get_battle());
	player.set_defending(false);
	player.set_last_action(FightAction::NONE);
	player.set_battle(nullptr);
	if (player.get_stats().current_hp == 0)
		player.respawn(world.get_spawn_room());
}

static void		cmd_fight(Player& player)
{
	// ? REVIEW: All this logic.
	Room	*player_room;
	Enemy	*enemy;

	if (player.get_battle())
	{
		player.send_to_outbox("You are already in a battle.");
		return ;
	}
	player_room = player.get_current_room();
	if (!player_room)
	{
		player.send_to_outbox("You must be in a room with an enemy to start a fight.");
		return ;
	}
	enemy = get_enemy_in_room(*player_room);
	if (!enemy)
		player.send_to_outbox("There is no enemy in this room.");
	else if (player.is_enemy_beaten(enemy))
		player.send_to_outbox("You have beaten the enemy already.");
	else
	{
		log("'" + player.get_name() + "' started a fight with '" + enemy->get_name() + "'.", LogLevel::INFO);
		player.set_battle(new Battle(player, enemy));
	}
}

static void		cmd_in_fight(Player& player, FightChoice choice, World& world)
{
	if (player.get_battle())
	{
		if (choice.action == FightAction::DEFEND && !player.get_shield())
			player.send_to_outbox("Can't defend, no shield equipped.");
		else
		{
			player.get_battle()->execute_turn(choice);
			if (player.get_battle()->is_finished())
				fight_result(player, world);
		}
	}
	else
		player.send_to_outbox("You are not in a battle.");
}

void	CommandHandler::handle(Player& player, World& world, std::string text)
{
	// TODO: Parse...
	// TODO: Add specific errors in case something wrong happens.
	// TODO: Validate if player is in a fight.
	// ? REVIEW: Recheck messages sent to the user.
	if (text == "MOVE NORTH")
		player.move(Direction::NORTH);
	else if (text == "MOVE EAST")
		player.move(Direction::EAST);
	else if (text == "MOVE SOUTH")
		player.move(Direction::SOUTH);
	else if (text == "MOVE WEST")
		player.move(Direction::WEST);
	else if (text == "FIGHT")
		cmd_fight(player);
	else if (text == "ATTACK")
		cmd_in_fight(player, {FightAction::ATTACK}, world);
	else if (text == "DEFEND")
		cmd_in_fight(player, {FightAction::DEFEND}, world);
	else if (text == "FLEE")
		cmd_in_fight(player, {FightAction::FLEE}, world);
	else if (text == "CONSUME APPLE")
	{
		// TODO: Find item, send it through the choice...
		cmd_in_fight(player, {FightAction::CONSUME}, world);
	}
	// TODO: MORE COMMANDS...
}

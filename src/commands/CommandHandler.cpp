#include "commands/CommandHandler.hpp"

#include <stdexcept>
#include <fstream>

#include "utils/utils.hpp"
#include "utils/types.hpp"
#include "battle/Battle.hpp"
#include "characters/Player.hpp"
#include "characters/Enemy.hpp"
#include "libs/json.hpp"
#include "protocol/responses.hpp"
#include "server/PlayerConnection.hpp"
#include "server/Server.hpp"
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

static void		cmd_look(const Command& cmd, Player& player)
{
	std::string	result;

	if (cmd.args.size() != 0)
		result = err(ErrorCode::WRONG_ARGUMENTS);
	else if (!player.get_current_room())
		result = err(ErrorCode::NO_ROOM);
	else
		result = ok(player.get_current_room()->look());
	player.send_to_outbox(result);

	// TODO: Remove the json test.
	// std::string	json_format;
	// json_format = player.get_current_room()->look();
	// nlohmann::json	j = nlohmann::json::parse(json_format);
	// std::ofstream	result_file("datos.json");
	// result_file << j.dump(4);
	// result_file.close();
}

static void		cmd_move(const Command& cmd, Player& player)
{
	Direction	dir;

	if (cmd.args.size() != 1)
	{
		player.send_to_outbox(err(ErrorCode::WRONG_ARGUMENTS));
		return;
	}
	dir = string_to_direction(cmd.args[0]);
	if (dir == Direction::INVALID)
	{
		player.send_to_outbox(err(ErrorCode::INVALID_ARGUMENT));
		return;
	}
	if (!player.move(dir))
		player.send_to_outbox(err(ErrorCode::NO_EXIT));
	else if (player.get_current_room())
		player.send_to_outbox(ok("room=" + player.get_current_room()->get_name()));
	else
		player.send_to_outbox(err(ErrorCode::UNEXPECTED_ERROR));
}

static void		cmd_quit(const Command& cmd, PlayerConnection& conn)
{
	if (cmd.args.size() != 0)
		conn.get_player().send_to_outbox(err(ErrorCode::WRONG_ARGUMENTS));
	else
	{
		conn.get_player().send_to_outbox(ok("bye"));
		conn.set_quitting(true);
	}
}

static void		cmd_fight(Player& player)
{
	// ? REVIEW: All this logic and messages.
	Room	*player_room;
	Enemy	*enemy;

	if (player.get_battle())
	{
		player.send_to_outbox("You are already in a battle.");
		return;
	}
	player_room = player.get_current_room();
	if (!player_room)
	{
		player.send_to_outbox("You must be in a room with an enemy to start a fight.");
		return;
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
	// ? REVIEW: Messages
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

static void		cmd_help(const Command& cmd, PlayerConnection& conn)
{
	std::string	result;

	if (!conn.get_server())
		result = err(ErrorCode::UNEXPECTED_ERROR);
	else if (cmd.args.size() != 0)
		result = err(ErrorCode::WRONG_ARGUMENTS);
	else
		result = ok(conn.get_server()->get_commands_instructions());
	conn.get_player().send_to_outbox(result);
}

void	CommandHandler::handle(const Command& cmd, PlayerConnection& connection, World& world)
{
	// TODO: Add specific errors in case something wrong happens.
	// TODO: Validate if player is in a fight.
	// ? REVIEW: Recheck messages sent to the user.
	Player&	player = connection.get_player();

	switch (cmd.type)
	{
		case CommandType::CONNECT:
			// Connection should be done only once in the client_thread.
			player.send_to_outbox(ok("already connected"));
			break;
		case CommandType::LOOK:
			cmd_look(cmd, player);
			break;
		case CommandType::MOVE:
			cmd_move(cmd, player);
			break;
		case CommandType::QUIT:
			cmd_quit(cmd, connection);
			break;
		default:
			// It should never get to this point, all types should be managed.
			player.send_to_outbox(err(ErrorCode::UNEXPECTED_ERROR));
	}
	// TODO: MORE COMMANDS...
}

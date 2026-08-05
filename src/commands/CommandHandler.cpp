#include "commands/CommandHandler.hpp"

#include <stdexcept>
#include <fstream>

#include "utils/utils.hpp"
#include "utils/types.hpp"
#include "battle/Battle.hpp"
#include "characters/Enchanter.hpp"
#include "characters/Enemy.hpp"
#include "characters/Player.hpp"
#include "characters/QuestGiver.hpp"
#include "characters/Vendor.hpp"
#include "items/chest/Chest.hpp"
#include "items/chest/ChestKey.hpp"
#include "items/Consumable.hpp"
#include "libs/json.hpp"
#include "protocol/events.hpp"
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
	assert(player.get_battle() != nullptr && "Cannot use this function if battle is nullptr");
	Enemy	*enemy;

	enemy = player.get_battle()->get_original_enemy();
	assert(enemy != nullptr && "Battles only have red fighter as enemy for now");
	player.reset_stats();
	if (player.get_battle()->get_winner() == &player)
	{
		// Player wins
		player.gain_gold(enemy->get_gold());
		player.add_beaten_enemy(enemy->get_id());

		// Red will always be the enemy for now.
		player.complete_defeat_quests(enemy->get_name());

		// For now, player always levels up after defeating an enemy
		player.level_up();
	}
	else if (player.get_battle()->get_winner() == nullptr)
		player.lose_gold(player.get_gold() / 5);	// Fleeing has losing gold as a penalty.
	delete (player.get_battle());
	player.set_defending(false);
	player.set_last_action(FightAction::NONE);
	player.set_battle(nullptr);
	if (player.get_stats().current_hp == 0)
		player.respawn(world.get_spawn_room());
}

static bool		is_allowed_in_battle(CommandType type)
{
	if (type == CommandType::ATTACK || type == CommandType::DEFEND ||
			type == CommandType::FLEE || type == CommandType::CONSUME ||
			type == CommandType::STATUS || type == CommandType::QUIT ||
			type == CommandType::CHAT)
		return (true);
	return (false);
}

static void		cmd_look(Player& player)
{
	std::string	result;

	if (!player.get_current_room())
		result = err(ErrorCode::NO_ROOM);
	else
		result = ok(player.get_current_room()->look());
	player.send_to_outbox(result);
}

static void		cmd_move(const Command& cmd, Player& player)
{
	Direction	dir;
	Room		*old_room;
	Room		*new_room;

	old_room = player.get_current_room();
	if (!old_room)
		return;
	dir = string_to_direction(cmd.args[0]);
	if (dir == Direction::INVALID)
	{
		player.send_to_outbox(err(ErrorCode::INVALID_ARGUMENT));
		return;
	}
	if (!player.move(dir))
		player.send_to_outbox(err(ErrorCode::NO_EXIT));
	else
	{
		new_room = player.get_current_room();
		assert(new_room != nullptr && "After move, the player must have a room");
		old_room->room_broadcast(evt_presence_leave(player.get_name()), &player);
		new_room->room_broadcast(evt_presence_enter(player.get_name()), &player);
		player.send_to_outbox(ok("room=" + new_room->get_id()));
		player.complete_area_quests(new_room->get_id());
	}
}

static void		cmd_quit(PlayerConnection& conn)
{
	if (!conn.get_server())
		return;
	conn.get_player().send_to_outbox(ok("bye"));
	conn.set_quitting(true);
}

static void		cmd_chat(const Command& cmd, PlayerConnection& conn)
{
	std::string	channel;
	std::string	msg;
	std::string	name;

	if (!conn.get_server())
		return;
	channel = cmd.args[0];
	msg = cmd.args[1];
	name = conn.get_player().get_name();
	if (channel == "GLOBAL")
	{
		conn.get_server()->broadcast(evt_global_chat(name, msg), conn.get_client_fd());
		conn.get_player().send_to_outbox(ok(""));
	}
	else if (channel == "ROOM")
	{
		Room	*room = conn.get_player().get_current_room();
		if (!room)
			conn.get_player().send_to_outbox(err(ErrorCode::NO_ROOM));
		else
		{
			room->room_broadcast(evt_room_chat(name, msg), &conn.get_player());
			conn.get_player().send_to_outbox(ok(""));
		}
	}
	else if (channel == "GROUP")
	{
		if (!conn.get_group())
			conn.get_player().send_to_outbox(err(ErrorCode::NOT_IN_GROUP));
		else
		{
			conn.get_server()->group_broadcast(*conn.get_group(), evt_group_chat(name, msg), conn.get_client_fd());
			conn.get_player().send_to_outbox(ok(""));
		}
	}
	else
		conn.get_player().send_to_outbox(err(ErrorCode::INVALID_ARGUMENT));
}

static void		cmd_who(PlayerConnection& conn)
{
	size_t	n_clients;

	if (!conn.get_server())
		return;
	n_clients = conn.get_server()->count_clients();
	conn.get_player().send_to_outbox(ok(std::to_string(n_clients)));
}

static void		cmd_group(const Command& cmd, PlayerConnection& conn)
{
	std::string	scope;
	std::string	result;

	if (!conn.get_server())
		return;
	scope = cmd.args[0];
	if (scope == "CREATE")
	{
		Group	*group = conn.get_server()->create_group(conn);
		if (group)
			result = ok("group=" + group->get_id());
		else
			result = err(ErrorCode::ALREADY_IN_GROUP);
	}
	else if (scope == "INVITE")
	{
		PlayerConnection	*target;
		target = conn.get_server()->search_client_by_name(cmd.args[1]);

		if (!conn.get_group())
			result = err(ErrorCode::NOT_IN_GROUP);
		else if (!target)
			result = err(ErrorCode::PLAYER_NOT_FOUND);
		else if (!conn.get_server()->invite_group(conn, *target))
			result = err(ErrorCode::ALREADY_INVITED);
		else
		{
			result = ok("");
			Group	*g = conn.get_group();
			target->get_player().send_to_outbox(evt_group_invite(g->get_leader()->get_player().get_name()));
		}
	}
	else if (scope == "JOIN")
	{
		const std::string	leader_name = cmd.args[1];
		Group				*group;
		PlayerConnection	*leader;

		leader = conn.get_server()->search_client_by_name(leader_name);
		if (conn.get_group())
			result = err(ErrorCode::ALREADY_IN_GROUP);
		else if (!leader)
			result = err(ErrorCode::PLAYER_NOT_FOUND);
		else
		{
			if (!conn.get_server()->find_group_by_leader(*leader))
				result = err(ErrorCode::GROUP_NOT_FOUND);
			else
			{
				group = conn.get_server()->join_group(conn, *leader);
				if (group)
				{
					result = ok("group=" + group->get_id());
					conn.get_server()->group_broadcast(*group, evt_group_join(conn.get_player().get_name()), conn.get_client_fd());
				}
				else
					result = err(ErrorCode::NOT_INVITED);
			}
		}

	}
	else if (scope == "KICK")
	{
		if (!conn.get_group())
			result = err(ErrorCode::NOT_IN_GROUP);
		else if (conn.get_group()->get_leader() != &conn)
			result = err(ErrorCode::NOT_LEADER);
		else
		{
			PlayerConnection	*target;

			target = conn.get_server()->search_client_by_name(cmd.args[1]);
			if (!target)
				result = err(ErrorCode::PLAYER_NOT_FOUND);
			else if (target == &conn)
				result = err(ErrorCode::CANT_KICK_SELF);
			else if (target->get_group() != conn.get_group())
				result = err(ErrorCode::TARGET_NOT_IN_GROUP);
			else
			{
				conn.get_server()->group_broadcast(*conn.get_group(), evt_group_leave(target->get_player().get_name()), target->get_client_fd());
				conn.get_server()->leave_group(*target);
				result = ok("");
			}
		}
	}
	else if (scope == "LEAVE")
	{
		if (!conn.get_group())
			result = err(ErrorCode::NOT_IN_GROUP);
		else
		{
			Group	*group = conn.get_group();
			conn.get_server()->group_broadcast(*group, evt_group_leave(conn.get_player().get_name()), conn.get_client_fd());
			if (conn.get_server()->leave_group(conn))
				result = ok("");
			else
				result = err(ErrorCode::NOT_IN_GROUP);
		}
	}
	conn.get_player().send_to_outbox(result);
}

static void		cmd_take(const Command& cmd, Player& player)
{
	Item	*taken;

	taken = player.obtain_item(cmd.args[0]);
	if (taken)
		player.send_to_outbox(ok("taken=" + taken->get_id()));
	else
		player.send_to_outbox(err(ErrorCode::ITEM_NOT_FOUND));
}

static void		cmd_drop(const Command& cmd, Player& player)
{
	Item	*dropped;

	dropped	= player.drop_item(cmd.args[0]);
	if (dropped)
		player.send_to_outbox(ok("dropped=" + dropped->get_id()));
	else
		player.send_to_outbox(err(ErrorCode::ITEM_NOT_IN_INVENTORY));
}

static void		cmd_inventory(Player& player)
{
	player.send_to_outbox(ok(player.get_inventory().to_json_format()));
}

static void		cmd_talk(const Command& cmd, Player& player)
{
	Room	*room;
	NPC		*npc;

	room = player.get_current_room();
	npc = nullptr;
	if (room)
		npc = room->get_NPC();
	if (!npc || npc->get_name() != cmd.args[0])
		player.send_to_outbox(err(ErrorCode::NPC_NOT_FOUND));
	else
		player.send_to_outbox(ok(npc->on_talk(player)));
}

/**
 * @brief	This functions includes ATTACK, DEFEND and FLEE commands.
 */
static void		cmd_fight(Player& player)
{
	Room	*player_room;
	Enemy	*enemy;

	if (player.get_battle())
	{
		player.send_to_outbox(err(ErrorCode::ALREADY_IN_BATTLE));
		return;
	}
	player_room = player.get_current_room();
	if (!player_room)
	{
		player.send_to_outbox(err(ErrorCode::NO_ROOM));
		return;
	}
	enemy = get_enemy_in_room(*player_room);
	if (!enemy)
		player.send_to_outbox(err(ErrorCode::NO_ENEMY));
	else if (player.is_enemy_beaten(enemy))
		player.send_to_outbox(err(ErrorCode::ALREADY_BEATEN));
	else
	{
		log("'" + player.get_name() + "' started a fight with '" + enemy->get_name() + "'.", LogLevel::INFO);
		player.set_battle(new Battle(player, enemy));
		player.send_to_outbox(ok("fight started"));
	}
}

static void		cmd_in_fight(const Command& cmd, Player& player, FightChoice choice, World& world)
{
	// CONSUME case
	if (choice.action == FightAction::CONSUME)
	{
		Item		*item = player.get_inventory().find_item_by_name(cmd.args[0]);
		Consumable	*consumable = dynamic_cast<Consumable*>(item);
		if (consumable)
			choice.consumable = consumable;
		else
		{
			player.send_to_outbox(err(ErrorCode::ITEM_NOT_IN_INVENTORY));
			return;
		}
	}

	// Executing action
	if (player.get_battle())
	{
		if (choice.action == FightAction::DEFEND && !player.get_shield())
			player.send_to_outbox(err(ErrorCode::NO_SHIELD));
		else if (choice.action == FightAction::DEFEND && player.get_last_action() == FightAction::DEFEND)
			player.send_to_outbox(err(ErrorCode::SHIELD_ON_COOLDOWN));
		else
		{
			player.get_battle()->execute_turn(choice);
			if (player.get_battle()->is_finished())
			{
				if (player.get_battle()->get_winner() == &player)
					player.send_to_outbox(ok("victory"));
				else
					player.send_to_outbox(ok("defeat"));
				fight_result(player, world);
			}
			else
				player.send_to_outbox(ok(player.get_battle()->to_json_format()));
		}
	}
	else
		player.send_to_outbox(err(ErrorCode::NOT_IN_BATTLE));
}

static void		cmd_status(Player& player)
{
	player.send_to_outbox(ok(player.status_json()));
}

static void		cmd_quest(const Command& cmd, Player& player)
{
	Room		*room;
	NPC			*npc;
	QuestGiver	*giver;
	std::string	quest_json;

	room = player.get_current_room();
	if (!room)
	{
		player.send_to_outbox(err(ErrorCode::NO_ROOM));
		return;
	}
	npc = room->get_NPC();
	if (!npc || cmd.args[0] != npc->get_name())
	{
		player.send_to_outbox(err(ErrorCode::NPC_NOT_FOUND));
		return;
	}
	giver = dynamic_cast<QuestGiver*>(npc);
	if (!giver)
	{
		player.send_to_outbox(err(ErrorCode::NO_QUEST_AVAILABLE));
		return;
	}
	quest_json = giver->request_quest(player);
	if (quest_json.empty())
		player.send_to_outbox(err(ErrorCode::NO_QUEST_AVAILABLE));
	else
		player.send_to_outbox(ok(quest_json));
}

static void		cmd_quests(Player& player)
{
	player.send_to_outbox(ok(player.quests_to_json_format()));
}

static void		cmd_open(Player& player)
{
	Room	*room;
	Chest	*chest;

	room = player.get_current_room();
	if (!room)
	{
		player.send_to_outbox(err(ErrorCode::NO_ROOM));
		return;
	}
	chest = room->get_chest();
	if (!chest)
	{
		player.send_to_outbox(err(ErrorCode::NO_CHEST));
		return;
	}
	else if (chest->is_opened())
		player.send_to_outbox(err(ErrorCode::CHEST_ALREADY_OPENED));
	else if (chest->get_guardian() && !player.is_enemy_beaten(chest->get_guardian()))
		player.send_to_outbox(err(ErrorCode::GUARDIAN_NOT_BEATEN));
	else if (!player.get_inventory().find_item<ChestKey>())
		player.send_to_outbox(err(ErrorCode::NO_KEY));
	else
	{
		std::list<Item*>	loot = chest->interact(player);
		if (!loot.empty())
			player.send_to_outbox(ok("chest opened"));
	}
}

static void		cmd_buy(const Command& cmd, Player& player)
{
	Room	*room;
	NPC		*npc;
	Vendor	*vendor;

	room = player.get_current_room();
	if (!room)
	{
		player.send_to_outbox(err(ErrorCode::NO_ROOM));
		return;
	}
	npc = room->get_NPC();
	if (!npc)
	{
		player.send_to_outbox(err(ErrorCode::NPC_NOT_FOUND));
		return;
	}
	vendor = dynamic_cast<Vendor*>(npc);
	if (!vendor)
		player.send_to_outbox(err(ErrorCode::NPC_NOT_VENDOR));
	else if (vendor->on_buy(player, cmd.args[0]))
		player.send_to_outbox(ok("bought " + cmd.args[0]));
	else
		player.send_to_outbox(err(ErrorCode::PURCHASE_FAILED));
}

static void		cmd_enchant(const Command& cmd, Player& player)
{
	Room		*room;
	NPC			*npc;
	Enchanter	*enchanter;

	room = player.get_current_room();
	if (!room)
	{
		player.send_to_outbox(err(ErrorCode::NO_ROOM));
		return;
	}
	npc = room->get_NPC();
	if (!npc)
	{
		player.send_to_outbox(err(ErrorCode::NPC_NOT_FOUND));
		return;
	}
	enchanter = dynamic_cast<Enchanter*>(npc);
	if (!enchanter)
		player.send_to_outbox(err(ErrorCode::NPC_NOT_ENCHANTER));
	else if (enchanter->on_enchant(player, cmd.args[1], cmd.args[0]))
		player.send_to_outbox(ok("enchanted " + cmd.args[1]));
	else
		player.send_to_outbox(err(ErrorCode::ENCHANT_FAILED));
}

static void		cmd_help(PlayerConnection& conn)
{
	std::string	result;

	if (!conn.get_server())
		return;
	else
		result = ok(conn.get_server()->get_commands_instructions());
	conn.get_player().send_to_outbox(result);
}

void	CommandHandler::handle(const Command& cmd, PlayerConnection& connection, World& world)
{
	Player&	player = connection.get_player();

	// If player is in battle, only some specific commands will be allowed.
	if (player.get_battle() && !is_allowed_in_battle(cmd.type))
	{
		player.send_to_outbox(err(ErrorCode::IN_BATTLE));
		return;
	}
	switch (cmd.type)
	{
		case CommandType::CONNECT:
			// Connection should be done only once in the client_thread.
			player.send_to_outbox(ok("already connected"));
			break;
		case CommandType::LOOK:
			cmd_look(player);
			break;
		case CommandType::MOVE:
			cmd_move(cmd, player);
			break;
		case CommandType::QUIT:
			cmd_quit(connection);
			break;
		case CommandType::CHAT:
			cmd_chat(cmd, connection);
			break;
		case CommandType::WHO:
			cmd_who(connection);
			break;
		case CommandType::GROUP:
			cmd_group(cmd, connection);
			break;
		case CommandType::TAKE:
			cmd_take(cmd, player);
			break;
		case CommandType::DROP:
			cmd_drop(cmd, player);
			break;
		case CommandType::INVENTORY:
			cmd_inventory(player);
			break;
		case CommandType::TALK:
			cmd_talk(cmd, player);
			break;
		case CommandType::FIGHT:
			cmd_fight(player);
			break;
		case CommandType::ATTACK:
			cmd_in_fight(cmd, player, {FightAction::ATTACK}, world);
			break;
		case CommandType::DEFEND:
			cmd_in_fight(cmd, player, {FightAction::DEFEND}, world);
			break;
		case CommandType::FLEE:
			cmd_in_fight(cmd, player, {FightAction::FLEE}, world);
			break;
		case CommandType::CONSUME:
			cmd_in_fight(cmd, player, {FightAction::CONSUME}, world);
			break;
		case CommandType::STATUS:
			cmd_status(player);
			break;
		case CommandType::QUEST:
			cmd_quest(cmd, player);
			break;
		case CommandType::QUESTS:
			cmd_quests(player);
			break;
		case CommandType::OPEN:
			cmd_open(player);
			break;
		case CommandType::BUY:
			cmd_buy(cmd, player);
			break;
		case CommandType::ENCHANT:
			cmd_enchant(cmd, player);
			break;
		case CommandType::HELP:
			cmd_help(connection);
			break;
		default:
			// It should never get to this point, all types should be managed.
			player.send_to_outbox(err(ErrorCode::UNEXPECTED_ERROR));
	}
}

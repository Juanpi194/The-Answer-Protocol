#include "protocol/responses.hpp"

std::string	err(ErrorCode err_code)
{
	std::string	result;

	switch (err_code)
	{
		case ErrorCode::NAME_IN_USE:
			result += "201 NAME_IN_USE";
			break;
		case ErrorCode::NOT_CONNECTED:
			result += "202 NOT_CONNECTED";
			break;
		case ErrorCode::BANNED_PLAYER:
			result += "203 BANNED_PLAYER";
			break;
		case ErrorCode::NO_EXIT:
			result += "301 NO_EXIT";
			break;
		case ErrorCode::NO_ROOM:
			result += "302 NO_ROOM";
			break;
		case ErrorCode::WRONG_ARGUMENTS:
			result += "400 WRONG_ARGUMENTS";
			break;
		case ErrorCode::NOT_IN_GROUP:
			result += "401 NOT_IN_GROUP";
			break;
		case ErrorCode::ALREADY_IN_GROUP:
			result += "402 ALREADY_IN_GROUP";
			break;
		case ErrorCode::INVALID_ARGUMENT:
			result += "403 INVALID_ARGUMENT";
			break;
		case ErrorCode::ITEM_NOT_FOUND:
			result += "404 ITEM_NOT_FOUND";
			break;
		case ErrorCode::ITEM_NOT_IN_INVENTORY:
			result += "404 ITEM_NOT_IN_INVENTORY";
			break;
		case ErrorCode::NPC_NOT_FOUND:
			result += "404 NPC_NOT_FOUND";
			break;
		case ErrorCode::NPC_NOT_HOSTILE:
			result += "405 NPC_NOT_HOSTILE";
			break;
		case ErrorCode::NO_QUEST_AVAILABLE:
			result += "406 NO_QUEST_AVAILABLE";
			break;
		case ErrorCode::INVALID_COMMAND:
			result += "407 INVALID_COMMAND";
			break;
		case ErrorCode::CONNECTION_FAILED:
			result += "900 CONNECTION_FAILED";
			break;
		case ErrorCode::SEND_FAILED:
			result += "901 SEND_FAILED";
			break;
		default:
			result += "Invalid error code";
	}
	return ("ERR" + result + '\n');
}

std::string	get_err_description(ErrorCode err_code)
{
	std::string	result;

	switch (err_code)
	{
		case ErrorCode::NAME_IN_USE:
			result += "Requested username already taken";
			break;
		case ErrorCode::NOT_CONNECTED:
			result += "User still needs authentication";
			break;
		case ErrorCode::BANNED_PLAYER:
			result += "Client with that username is banned";
			break;
		case ErrorCode::NO_EXIT:
			result += "Invalid movement direction";
			break;
		case ErrorCode::NO_ROOM:
			result += "Player must be in a room";
			break;
		case ErrorCode::WRONG_ARGUMENTS:
			result += "Command received wrong number of arguments.";
			break;
		case ErrorCode::NOT_IN_GROUP:
			result += "Group operation requires group membership";
			break;
		case ErrorCode::ALREADY_IN_GROUP:
			result += "Player already belongs to a group";
			break;
		case ErrorCode::INVALID_ARGUMENT:
			result += "Command received invalid an invalid argument";
			break;
		case ErrorCode::ITEM_NOT_FOUND:
			result += "Requested item not available in room";
			break;
		case ErrorCode::ITEM_NOT_IN_INVENTORY:
			result += "Requested item not in player inventory";
			break;
		case ErrorCode::NPC_NOT_FOUND:
			result += "Requested NPC not present in room";
			break;
		case ErrorCode::NPC_NOT_HOSTILE:
			result += "NPC cannot be attacked (not an enemy)";
			break;
		case ErrorCode::NO_QUEST_AVAILABLE:
			result += "NPC has no quests or quest already completed";
			break;
		case ErrorCode::INVALID_COMMAND:
			result += "Command is not supported";
			break;
		case ErrorCode::CONNECTION_FAILED:
			result += "Connection establishment failed";
			break;
		case ErrorCode::SEND_FAILED:
			result += "Message transmission failed";
			break;
		default:
			result += ("Invalid error code");
	}
	return ("ERR DESC: " + result + '\n');
}

std::string	ok(const std::string& msg)
{
	return ("OK " + msg + "\n");
}

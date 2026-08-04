#pragma once
#include <string>

#include "utils/attributes.hpp"

enum class ErrorCode
{
	NAME_IN_USE,
	NOT_CONNECTED,
	BANNED_PLAYER,
	NO_EXIT,
	NO_ROOM,
	WRONG_ARGUMENTS,
	NOT_IN_GROUP,
	ALREADY_IN_GROUP,
	INVALID_ARGUMENT,
	ITEM_NOT_FOUND,
	ITEM_NOT_IN_INVENTORY,
	NPC_NOT_FOUND,
	PLAYER_NOT_FOUND,
	GROUP_NOT_FOUND,
	NPC_NOT_HOSTILE,
	NO_QUEST_AVAILABLE,
	INVALID_COMMAND,
	NO_SHIELD,
	NOT_IN_BATTLE,
	ALREADY_IN_BATTLE,
	NO_ENEMY,
	ALREADY_BEATEN,
	ALREADY_INVITED,
	NOT_INVITED,
	IN_BATTLE,
	NPC_NOT_VENDOR,
	NPC_NOT_ENCHANTER,
	NO_CHEST,
	CHEST_ALREADY_OPENED,
	GUARDIAN_NOT_BEATEN,
	NO_KEY,
	CONNECTION_FAILED,
	SEND_FAILED,
	UNEXPECTED_ERROR
};

/**
 * @brief	Returns a string that matches the specified error code.
 * @param	err_code	The error code.
 * @returns	The error code in string, with its code and name.
 */
std::string	err(ErrorCode err_code);

/**
 * @brief	Gets the description of the specified error code.
 * @param	err_code	The error code.
 * @returns	A string with the description of the error code.
 */
std::string	get_err_description(ErrorCode err_code) TAP_ERROR("Unused in this project, but useful.");

/**
 * @brief	Creates a string that starts with an 'ok' message,
 * 			followed by the given text.
 * @param	msg	The text to be followed by the 'ok' message.
 * @returns	The whole new string.
 */
std::string	ok(const std::string& msg);

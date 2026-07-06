#pragma once
#include <string>

enum LogLevel
{
	NONE,
	ERROR,
	WARNING,
	INFO,
	DEBUG
};

enum Color
{
	RESET = 0,

	BLACK = 30,
	RED = 31,
	GREEN = 32,
	YELLOW = 33,
	BLUE = 34,
	MAGENTA = 35,
	CYAN = 36,
	WHITE = 37,

	BRIGHT_RED = 91,
	BRIGHT_GREEN = 92,
	BRIGHT_YELLOW = 93,
	BRIGHT_BLUE = 94,
	BRIGHT_MAGENTA = 95,
	BRIGHT_CYAN = 96,
	BRIGHT_WHITE = 97
};

/**
 * @brief	This variable is set when the program is compiled.
 * 			If the program is compiled with the flag `DEBUG_BUILD`,
 * 			the value will be set to `DEBUG`, otherwise `WARNING` level
 * 			will be set.
 * @note	Check `globals.cpp`.
*/
extern LogLevel	current_level;

/**
 * @brief		Prints the given text if the log level is accepted in the
 * 				specified level. If no level is specified, current log level
 * 				of the program will be used.
 * 				A new line character is printed at the end.
 * @param		msg		Message to be printed.
 * @param		level	Log level to print `msg`.
 * @example	log("Hello", LogLevel::LOG_ERROR);
 * @note		All logs will be printed in the `cerr` (2).
 */
void		log(const std::string& msg, LogLevel level = current_level);

/**
 * @brief	All space characters at the beginning and at the end will be removed.
 * 			Available characters to remove:
 * 
 * 			- ' ' (Space)
 * 
 * 			- '\n' (New line)
 * 
 * 			- '\r' (Carriage return)
 * 
 * 			If it is specified, characters in the middle of the string will be
 * 			removed as well.
 * @param		s		Reference to the string to trim.
 * @param		middle	Set as `true` by default. If `true`, space characters
 * 						in the middle of the string will be removed too. Otherwise,
 * 						only characters at the end and at the beginning will be removed.
 * @example	trim_str(" hello    world    ") -> s = "hello world"
 * 			trim_str(" hello    world    ", false) -> s = "hello    world"	
*/
void		trim_str(std::string& s, bool middle = true);

/**
 * @brief	Checks whether a string starts with a given prefix.
 * @param str	The string to check.
 * @param prefix	The prefix to look for at the beginning of `str`.
 * @returns	`true` if `str` begins with `prefix`, `false` otherwise.
*/
bool		starts_with(const std::string& str, const std::string& prefix);

/**
 * @brief	Checks whether a string ends with a given suffix.
 * @param str	The string to check.
 * @param suffix	The suffix to look for at the end of `str`.
 * @returns `true` if `str` ends with `suffix`, `false` otherwise.
*/
bool		ends_with(const std::string& str, const std::string& suffix);

/**
 * @brief	Checks whether a word starts with an uppercase letter and the
 * 			rest are lowercase, or not.
 * @param	str	String to be evaluated.
 * @returns	`true` if is a title. `false` otherwise.
*/
bool		is_title(const std::string& str);

/**
 * @brief	Colors the given string with the color. 
 * 			`color` is `RESET` by default, which means no color will be used.
 * @param	str		The string to be colored.
 * @param	color	The color to be used. Check the `Color` enum to see
 * 					available colors.
 * @returns	A new string with the given string colored.
*/
std::string	color_str(const std::string& str, Color color = Color::RESET);



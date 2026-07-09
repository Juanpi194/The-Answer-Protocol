#include "world/World.hpp"

#include <stdexcept>

#include "utils/utils.hpp"

bool	World::validate_name(const std::string& name)
{
	std::string	temp = name;

	trim_str(temp);
	if (temp.empty())
		return (log("World name cannot be empty.", LogLevel::ERROR), false);
	if (temp.size() < MIN_NAME_LENGTH)
		return (log("World name is too short (MIN CHARACTERS: " + std::to_string(MIN_NAME_LENGTH) + ").", LogLevel::ERROR), false);
	if (temp.size() > MAX_NAME_LENGTH)
		return (log("World name is too long (MAX CHARACTERS: " + std::to_string(MAX_NAME_LENGTH) + ").", LogLevel::ERROR), false);
	if (TITLE_NAME && !is_title(temp))
		return (log("World name must be have title format.", LogLevel::ERROR), false);
	return (true);
}

bool	World::validate_json(const std::string& json_path)
{
	if (!ends_with(json_path, ".json") && !ends_with(json_path, ".jsonc"))
		return (log("Provided json file path sufix must be '.json' or '.jsonc'.", LogLevel::ERROR), false);
	// TODO: Implement validation logic
	return (true);
}

// Constructors ---------------------------------------------------------------

World::World(const std::string& name):
	name(name)
{
	if (!validate_name(name))
		throw std::invalid_argument("World name validation failed.");
	// TODO: Create default world
}

World::World(const std::string& name, const std::string& json_path)
{
	if (!validate_name(name) || !validate_json(json_path))
		throw std::invalid_argument("World validation failed.");
	// TODO: Create world with json config
}

World::~World(void)
{
	// TODO: Delete all npcs, items, ...
}

// Getters and setters --------------------------------------------------------

std::string				World::get_name(void) const noexcept
{
	return (name);
}

std::list<Room*>&		World::get_rooms(void) noexcept
{
	return (rooms);
}

const std::list<Room*>&	World::get_rooms(void) const noexcept
{
	return (rooms);
}

// Utils ----------------------------------------------------------------------

void	World::reset_world(void)
{
	// TODO: Reset all rooms, npcs, ...
}

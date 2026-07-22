#include "parser/jsonconfig.hpp"

#include <fstream>
#include <sstream>
#include <stdexcept>

std::string JsonConfig::read_file(const std::string& path)
{
	std::ifstream file(path);

	if (!file.is_open())
		throw std::runtime_error("Could not open file: " + path);

	std::stringstream buffer;
	buffer << file.rdbuf();

	return (buffer.str());
}

void JsonConfig::validate(const nlohmann::json& json_data)
{
	//WORLD

	if (!json_data.contains("world"))
		throw std::runtime_error("Missing 'world' section.");

	if (!json_data["world"].is_object())
		throw std::runtime_error("'world' must be an object.");

	if (!json_data["world"].contains("name"))
		throw std::runtime_error("Missing 'world.name'.");

	if (!json_data["world"].contains("spawn"))
		throw std::runtime_error("Missing 'world.spawn'.");

	//ROOMS 

	if (!json_data.contains("rooms"))
		throw std::runtime_error("Missing 'rooms' section.");

	if (!json_data["rooms"].is_array())
		throw std::runtime_error("'rooms' must be an array.");

	//ITEMS

	if (!json_data.contains("items"))
		throw std::runtime_error("Missing 'items' section.");

	if (!json_data["items"].is_object())
		throw std::runtime_error("'items' must be an object.");
 
	//ROOM EACH
	for (const auto& room : json_data["rooms"])
	{
		if (!room.is_object())
			throw std::runtime_error("Each room must be an object.");

		if (!room.contains("id"))
			throw std::runtime_error("A room is missing its id.");

		if (!room.contains("name"))
			throw std::runtime_error("Room '" +
				room.value("id", "unknown") +
				"' is missing its name.");

		if (!room.contains("description"))
			throw std::runtime_error("Room '" +
				room.value("id", "unknown") +
				"' is missing its description.");

		if (!room.contains("exits"))
			throw std::runtime_error("Room '" +
				room.value("id", "unknown") +
				"' is missing its exits.");

		if (!room["exits"].is_object())
			throw std::runtime_error("Room '" +
				room.value("id", "unknown") +
				"' exits must be an object.");
	}
}

nlohmann::json JsonConfig::load_json(const std::string& path)
{
	std::string content;
	nlohmann::json json_data;

	content = read_file(path);

	json_data = nlohmann::json::parse(content);

	validate(json_data);

	return (json_data);
}
#ifndef ROOMPARSER_HPP
#define ROOMPARSER_HPP

#include <list>
#include <libs/json.hpp>

class Room;

class RoomParser
{
	public:
		static std::list<Room*>	parse(const nlohmann::json& rooms_json, const nlohmann::json& npcs_json);
};

#endif
#ifndef ITEMPARSER_HPP
#define ITEMPARSER_HPP

#include <map>
#include <string>
#include <libs/json.hpp>

class Item;

class ItemParser
{
	public:
		// MODIFIED: devuelve un mapa (clave del JSON -> Item*) en vez de
		// una lista plana -- el id que se calculaba antes ("item." + key)
		// no se le pasaba a ItemFactory::create_from_name, así que
		// Item::get_id() no sirve para relacionar cada entrada del JSON
		// con su Item* concreto. Con el mapa, RoomParser puede hacerlo
		// de forma fiable.
		std::map<std::string, Item*>	parse(const nlohmann::json& items_json);
};

#endif

#ifndef ITEMPARSER_HPP
#define ITEMPARSER_HPP

#include <list>
#include <libs/json.hpp>

class Item;

class ItemParser
{
	public:
		std::list<Item*>	parse(const nlohmann::json& items_json);
};

#endif
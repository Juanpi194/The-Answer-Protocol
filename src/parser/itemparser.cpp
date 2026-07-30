#include "parser/itemparser.hpp"

// #include "items/Consumable.hpp"
#include "factories/ItemFactory.hpp"

static Item *build_item(const std::string& id,
						const std::string& name,
						const std::string& description)
{
	return  (ItemFactory::create_from_name(name));
	// return (new Consumable(id, name, description));
}

std::list<Item*> ItemParser::parse(const nlohmann::json& items_json)
{
	std::list<Item*> items;

	if (!items_json.is_object())
		return (items);

	for (nlohmann::json::const_iterator it = items_json.begin();
			it != items_json.end(); ++it)
	{
		const nlohmann::json& item_json = *it;

		const std::string id = "item." + it.key();
		const std::string name = item_json["name"];
		const std::string description =
			item_json.value("description", "A useful object.");

		Item *item = build_item(id, name, description);

		if (item != nullptr)
			items.push_back(item);
	}

	return (items);
}
#include "parser/itemparser.hpp"

#include "items/Consumable.hpp"
#include "utils/utils.hpp"

static Item *build_item(const std::string& id,
						const std::string& name,
						const std::string& description,
						const std::string& type)
{
	if (type == "consumable")
		return (new Consumable(id, name, description));

	log("Item '" + id + "' has unsupported type '" + type + "'.",
		LogLevel::WARNING);

	return (nullptr);
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
		const std::string type =
			item_json.value("type", "consumable");

		Item *item = build_item(id, name, description, type);

		if (item != nullptr)
			items.push_back(item);
	}

	return (items);
}
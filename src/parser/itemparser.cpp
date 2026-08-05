#include "parser/itemparser.hpp"

// #include "items/Consumable.hpp"
#include "factories/ItemFactory.hpp"
#include "utils/utils.hpp"

static Item *build_item(const nlohmann::json& item_json)
{
	const std::string name = item_json["name"];
	const std::string description =
		item_json.value("description", "A useful object.");

	return  (ItemFactory::create_from_name(name));
	// return (new Consumable(id, name, description));
}

static void add_item(
	std::list<Item*>& items,
	const std::string& id,
	const nlohmann::json& item_json)
{
	try
	{
		Item *item = build_item(item_json);

		if (item != nullptr)
			items.push_back(item);
	}
	catch (const std::exception& e)
	{
		log(
			"Could not create item '"
			+ id + "': "
			+ e.what(),
			LogLevel::WARNING);
	}
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

		add_item(items, id, item_json);
	}

	return (items);
}

#include "items/Item.hpp"

#include <stdexcept>

#include "utils/utils.hpp"

unsigned int		Item::available_id = 0;
const std::string	Item::PREFIX = "item.";

bool	Item::validate_arguments(const std::string& name, const std::string& description)
{
	std::string	item_name;
	std::string	item_description;

	// NAME
	item_name = name;
	trim_str(item_name);
	if (item_name.empty())
		return (log("Item name cannot be empty.", LogLevel::ERROR), false);
	if (TITLE_NAME && !is_title(item_name))
		return (log("Item name is not a title.", LogLevel::ERROR), false);
	if (item_name.size() > MAX_NAME_LENGTH)
		return (log("Item name is too long (MAX CHARACTERS: " + std::to_string(MAX_NAME_LENGTH) + ").", LogLevel::ERROR), false);
	if (item_name.size() < MIN_NAME_LENGTH)
		return (log("Item name is too short (MIN CHARACTERS: " + std::to_string(MIN_NAME_LENGTH) + ").", LogLevel::ERROR), false);

	// DESCRIPTION
	item_description = description;
	trim_str(item_description);
	if (item_description.empty())
		return (log("Description cannot be empty.", LogLevel::ERROR), false);
	if (item_description.size() > MAX_DESCRIPTION_LENGTH)
		return (log("Item description is too long (MAX CHARACTERS: " + std::to_string(MAX_DESCRIPTION_LENGTH) + ").", LogLevel::ERROR), false);
	if (item_description.size() < MIN_DESCRIPTION_LENGTH)
		return (log("Item description is too short (MIN CHARACTERS: " + std::to_string(MIN_DESCRIPTION_LENGTH) + ").", LogLevel::ERROR), false);

	// VALIDATION PASSED
	return (true);
}

// Constructors ---------------------------------------------------------------

Item::Item(const std::string& name, const std::string& description):
	id(PREFIX + std::to_string(available_id++)),	
	name(name),
	description(description)
{
	if (!validate_arguments(name, description))
		throw std::invalid_argument("Item validation failed.");
}

Item::Item(const Item& item):
	id(PREFIX + std::to_string(available_id++)),
	name(item.name),
	description(item.description)
{
}

// Getters and setters --------------------------------------------------------

std::string	Item::get_id(void) const noexcept
{
	return (id);
}

std::string	Item::get_name(void) const noexcept
{
	return (name);
}

std::string	Item::get_description(void) const noexcept
{
	return (description);
}

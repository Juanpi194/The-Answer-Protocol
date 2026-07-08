#pragma once
#include <string>

class Enchantment
{
	private:
		std::string	name;
		std::string	description;

		static constexpr size_t	MIN_NAME_LENGTH = 3;
		static constexpr size_t	MAX_NAME_LENGTH = 20;
		static constexpr size_t	MIN_DESCRIPTION_LENGTH = 10;
		static constexpr size_t	MAX_DESCRIPTION_LENGTH = 80;

	public:
		// Constructors -------------------------------------------------------

		Enchantment(const std::string& name, const std::string& description);
		Enchantment(const Enchantment& enchantment);
		~Enchantment(void) = default;

		// Operators ----------------------------------------------------------

		Enchantment&	operator=(const Enchantment& other) = delete;

		// Getters and setters ------------------------------------------------

		std::string	get_name(void) const noexcept;
		std::string	get_description(void) const noexcept;

		// Utils --------------------------------------------------------------

		// TODO
};

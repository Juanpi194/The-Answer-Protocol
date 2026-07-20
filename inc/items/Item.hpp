#pragma once
#include <string>

class Item
{
	private:
		const std::string	id;
		const std::string	name;
		const std::string	description;

		static constexpr bool		TITLE_NAME = true;
		static constexpr size_t		MIN_NAME_LENGTH = 3;
		static constexpr size_t		MAX_NAME_LENGTH = 18;
		static constexpr size_t		MIN_DESCRIPTION_LENGTH = 8;
		static constexpr size_t		MAX_DESCRIPTION_LENGTH = 60;

		/**
		 * @brief	Verifies that the provided arguments follow the specified
		 * 			rules.
		 * @returns	`true` if all parameters follow the expected. `false` otherwise.
		 * @note	This method should ONLY be used in the constructor.
		 */
		static bool	validate_arguments(const std::string& name, const std::string& description);
	protected:
		static const std::string	PREFIX;	// Defined in Item.cpp
	public:
		// Constructors -------------------------------------------------------

		Item(const std::string& id, const std::string& name, const std::string& description);
		Item(const Item& item) = delete;
		virtual	~Item(void) = default;
		virtual Item*	clone(void) const noexcept = 0;

		// Operators ----------------------------------------------------------

		Item&	operator=(const Item& other) = delete;

		// Getters and setters ------------------------------------------------

		std::string	get_id(void) const noexcept;
		std::string	get_name(void) const noexcept;
		std::string	get_description(void) const noexcept;

		// Utils --------------------------------------------------------------

		// ! FIXME: Delete this method.
		// virtual void	use(void) noexcept = 0;
};

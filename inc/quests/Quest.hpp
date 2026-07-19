#pragma once
#include <string>

class Quest
{
	private:
		const std::string	name;
		const std::string	description;
		bool				completed;

		static constexpr size_t	MIN_NAME_LENGTH = 8;
		static constexpr size_t	MAX_NAME_LENGTH = 30;
		static constexpr size_t	MIN_DESCRIPTION_LENGTH = 15;
		static constexpr size_t	MAX_DESCRIPTION_LENGTH = 50;
	public:
		// Constructors -------------------------------------------------------

		Quest(const std::string& name, const std::string& description);
		Quest(const Quest& quest);
		~Quest(void) = default;

		// Operators ----------------------------------------------------------

		Quest&	operator=(const Quest& other) = delete;

		// Getters and setters ------------------------------------------------

		std::string	get_name(void) const noexcept;
		std::string	get_description(void) const noexcept;
		bool		is_completed(void) const noexcept;

		void		set_completed(bool completed) noexcept;

		// Utils --------------------------------------------------------------

		// TODO: Add utils functions
};

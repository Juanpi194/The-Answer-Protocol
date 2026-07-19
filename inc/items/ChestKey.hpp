#pragma once

#include "items/Item.hpp"

class ChestKey final: public Item
{
	private:
		static unsigned int				available_id;	// Defined in ChestKey.cpp
		static const std::string		PREFIX;	// Defined in ChestKey.cpp
	public:
		static const std::string		NAME;	// Defined in ChestKey.cpp
		static const std::string		DESCRIPTION;	// Defined in ChestKey.cpp

		// Constructors -------------------------------------------------------

		ChestKey(void);
		ChestKey(const ChestKey& chest_key);
		~ChestKey(void) = default;

		// Operators ----------------------------------------------------------

		ChestKey&	operator=(const ChestKey& other) = delete;

		// Getters and setters ------------------------------------------------

		// Utils --------------------------------------------------------------

		void	use(void) noexcept override;
};

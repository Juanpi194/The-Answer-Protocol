#pragma once //para incluir solo 1 vez el archivo hpp en la compilacion 

#include <string>

class JsonConfig
{
	public:
		static std::string	load_json(const std::string& path);

	private:
		static std::string	read_file(const std::string& path);
		static std::string	remove_comments(const std::string& input);
		static std::string	remove_trailing_commas(const std::string& input);
		static void			validate(const std::string& input);

		JsonConfig(void);
};
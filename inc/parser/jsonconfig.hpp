#ifndef JSONCONFIG_HPP
#define JSONCONFIG_HPP

#include <string>
#include <libs/json.hpp>

class JsonConfig
{
	public:
		std::string read_file(const std::string& path);
		nlohmann::json load_json(const std::string& path);
		void validate(const nlohmann::json& json_data);
};

#endif
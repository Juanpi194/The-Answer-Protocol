#pragma once

#include <string>
#include <libs/json.hpp>

class JsonConfig
{
	public:
		static std::string		read_file(const std::string& path);
		static nlohmann::json	load_json(const std::string& path);
		static void				validate(const nlohmann::json& json_data);
};
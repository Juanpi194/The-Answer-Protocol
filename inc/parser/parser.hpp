#pragma once

#include <string>

#include "libs/json.hpp"

/**
 * @brief	Parses the specified json file path.
 */
nlohmann::json	load_json(const std::string& path);

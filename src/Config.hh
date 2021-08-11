#pragma once

#include <nlohmann/json.hpp>
#include <fstream>

using Config = nlohmann::json;

inline Config read_from_file(const char* config_path) {
    std::ifstream file(config_path);
    return nlohmann::json::parse(file);
}
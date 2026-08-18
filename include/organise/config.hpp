#pragma once

#include <filesystem>
#include <nlohmann/json.hpp>

namespace organise {

using json = nlohmann::json;

std::filesystem::path getDefaultConfigPath();

// Loads the config file
json loadConfig(const std::filesystem::path& path);

}

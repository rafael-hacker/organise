#pragma once

#include <filesystem>
#include <nlohmann/json.hpp>

namespace organise {

void processDirectory(const std::filesystem::path& targetDir, const nlohmann::json& rules);

} // namespace organise

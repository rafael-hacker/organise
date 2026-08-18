#pragma once

#include "../include/options.hpp"
#include <filesystem>
#include <nlohmann/json.hpp>

namespace organise {

void processDirectory(const nlohmann::json& rules, const Options& opts);
void startWatcher(const nlohmann::json& rules, const Options& opts);

} // namespace organise

#pragma once

#include <string>
#include <filesystem>

namespace organise::log {

std::filesystem::path getLogPath(); /* get the log file path */

void logActivity(const std::string& message);

} // namespace organise::log

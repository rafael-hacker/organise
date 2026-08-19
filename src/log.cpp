#include "organise/log.hpp"
#include "organise/colors.hpp"
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <chrono>
#include <ctime>
#include <filesystem>

namespace organise::log {

std::filesystem::path getLogPath() {
    std::filesystem::path baseDir;
    const char* xdgState = std::getenv("XDG_STATE_HOME");
    if (xdgState && xdgState[0] != '\0') {
        baseDir = std::filesystem::path(xdgState);
    } else {
        const char* home = std::getenv("HOME");
        baseDir = home ? std::filesystem::path(home) / ".local" / "state"
                       : std::filesystem::current_path();
    }
    auto dir = baseDir / "organise";
    std::filesystem::create_directories(dir);
    return dir / "activity.log";
}

void logActivity(const std::string& message) {
    auto path = getLogPath();
    std::ofstream file(path, std::ios::app);
    if (!file.is_open()) {
        // Silently fail – don't spam the user if we can't write
        return;
    }

    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    char timestamp[32];
    std::strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", std::localtime(&time_t));

    file << "[" << timestamp << "] " << message << std::endl;
}

} // namespace organise::log

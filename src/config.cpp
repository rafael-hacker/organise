#include "../include/config.hpp"
#include <cstdlib>
#include <fstream>
#include <iostream>

namespace organise {

std::filesystem::path getDefaultConfigPath() {
    const char* homeDir = std::getenv("HOME");
    if (!homeDir) {
        return {};
    }
    return std::filesystem::path(homeDir) / ".config" / "organise" / "config.json";
}

json loadConfig(const std::filesystem::path& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Error: couldn't open config file in " << path << std::endl;
        return nullptr;
    }

    json data;
    file >> data;
    return data;
}

} // namespace organise

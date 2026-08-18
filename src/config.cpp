#include "organise/config.hpp"
#include "organise/colors.hpp"
#include <cstdlib>
#include <fstream>
#include <iostream>

namespace organise {

std::filesystem::path getDefaultConfigPath() {
    const char* xdgConfig = std::getenv("XDG_CONFIG_HOME");
    if (xdgConfig && xdgConfig[0] != '\0') {
        return std::filesystem::path(xdgConfig) / "organise" / "config.json";
    }

    const char* homeDir = std::getenv("HOME");
    if (!homeDir) {
        return {};
    }
    return std::filesystem::path(homeDir) / ".config" / "organise" / "config.json";
}

json loadConfig(const std::filesystem::path& path) {
    if (!std::filesystem::exists(path)) {
        std::cout << color::yellow << "Config file not found. Creating standard config in: " << path << color::reset << std::endl;

        if (path.has_parent_path()) {
            std::filesystem::create_directories(path.parent_path());
        }

        const char* home = std::getenv("HOME");
        std::string homeStr = home ? home : "";

        json defaultConfig = {
            {".pdf", homeStr + "/Documents/PDFs"},
            {".png", homeStr + "/Pictures"},
            {".jpg", homeStr + "/Pictures"},
            {".zip", homeStr + "/Downloads/Archives"}
        };

        std::ofstream outFile(path);
        if (outFile.is_open()) {
            outFile << defaultConfig.dump(4) << std::endl;
            outFile.close();
        } else {
            std::cerr << color::red << "Error: Couldn't create config file in: " << path <<  color::reset << std::endl;
            return nullptr;
        }

        return defaultConfig;
    }

    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << color::red << "Error: Couldn't open config file in: " << path << color::reset << std::endl;
        return nullptr;
    }

    json data;
    try {
        file >> data;
    } catch (const json::parse_error& e) {
        std::cerr << color::red << "Error parsing JSON config: " << e.what() << color::reset << std::endl;
        return nullptr;
    }

    return data;
}

} // namespace organise

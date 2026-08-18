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
    if (!std::filesystem::exists(path)) {
        std::cout << "Config file not found. Creating standard config in: " << path << std::endl;

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
            std::cerr << "\033[31mError: Couldn't create config file in: " << path <<  "\033[0m" << std::endl;
            return nullptr;
        }

        return defaultConfig;
    }

    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "\033[31mError: Couldn't open config file in: " << path << "\033[0m" << std::endl;
        return nullptr;
    }

    json data;
    try {
        file >> data;
    } catch (const json::parse_error& e) {
        std::cerr << "\033[31mError parsing JSON config: " << e.what() << "\033[0m" <<std::endl;
        return nullptr;
    }

    return data;
}

} // namespace organise

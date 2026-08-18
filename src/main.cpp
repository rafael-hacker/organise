#include "../include/config.hpp"
#include "../include/organiser.hpp"
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Usage: org <path>" << std::endl;
        return 1;
    }

    std::filesystem::path targetDir = argv[1];
    std::filesystem::path configPath = organise::getDefaultConfigPath();

    if (configPath.empty()) {
        std::cerr << "Error: HOME Variable not found." << std::endl;
        return 1;
    }

    auto rules = organise::loadConfig(configPath);
    if (rules.is_null()) {
        return 1;
    }

    organise::processDirectory(targetDir, rules);

    return 0;
}

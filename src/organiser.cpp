#include "../include/organiser.hpp"
#include <iostream>

namespace organise {

void processDirectory(const std::filesystem::path& targetDir, const nlohmann::json& rules) {
    if (!std::filesystem::exists(targetDir) || !std::filesystem::is_directory(targetDir)) {
        std::cerr << "Error: the directory " << targetDir << " doesn't exist or isn't a directory." << std::endl;
        return;
    }

    for (const auto& entry : std::filesystem::directory_iterator(targetDir)) {
        if (entry.is_regular_file()) {
            auto ext = entry.path().extension();
            auto filename = entry.path().filename();
            
            auto it = rules.find(ext);
            if (it != rules.end()) {
                std::filesystem::path destDir = it.value().get<std::filesystem::path>();

                if (!std::filesystem::exists(destDir)) {
                    std::filesystem::create_directories(destDir);
                }

                std::filesystem::path destPath = destDir / filename;
                std::filesystem::rename(entry.path(), destPath);
                std::cout << "Moved: " << filename << " -> " << destPath << std::endl;
            }
        }
    }
}

} // namespace organise

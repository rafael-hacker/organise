#include "../include/organiser.hpp"
#include <iostream>
#include <cctype>

namespace organise {

enum class ConflictAction {
    Rename,
    Skip,
    Overwrite
};

static std::filesystem::path getUniquePath(const std::filesystem::path& destPath) {
    auto parent = destPath.parent_path();
    auto stem = destPath.stem().string();
    auto ext = destPath.extension().string();

    int counter = 1;
    std::filesystem::path uniquePath;
    do {
        uniquePath = parent / (stem + " (" + std::to_string(counter) + ")" + ext);
        counter++;
    } while (std::filesystem::exists(uniquePath));

    return uniquePath;
}

static ConflictAction promptUserConflict(const std::filesystem::path& filename) {
    while (true) {
        std::cout << "The file '" << filename.string() << "' already exists.\n"
                  << "  [r]ename (as " << filename.stem().string() << " (1)" << filename.extension().string() << ")\n"
                  << "  [s]kip\n"
                  << "  [o]verwrite \n"
                  << "Choose [r/s/o]: ";

        char choice;
        std::cin >> choice;
        choice = static_cast<char>(std::tolower(choice));

        if (choice == 'r') return ConflictAction::Rename;
        if (choice == 's') return ConflictAction::Skip;
        if (choice == 'o') return ConflictAction::Overwrite;

        std::cout << "Invalid option! Try again.\n\n";
    }
}

void processDirectory(const std::filesystem::path& targetDir, const nlohmann::json& rules, bool dryRun) {
    if (!std::filesystem::exists(targetDir) || !std::filesystem::is_directory(targetDir)) {
        std::cerr << "Error: Invalid directory " << targetDir << std::endl;
        return;
    }

    for (const auto& entry : std::filesystem::directory_iterator(targetDir)) {
        if (entry.is_regular_file()) {
            auto ext = entry.path().extension();
            auto filename = entry.path().filename();

            auto it = rules.find(ext);
            if (it != rules.end()) {
                std::filesystem::path destDir = it.value().get<std::filesystem::path>();

                if (!dryRun && !std::filesystem::exists(destDir)) {
                    std::filesystem::create_directories(destDir);
                }

                std::filesystem::path destPath = destDir / filename;

                if (std::filesystem::exists(destPath)) {
                    if (dryRun) {
                        std::cout << "[DRY-RUN] Conflit detected for: " << filename << std::endl;
                        continue;
                    }

                    ConflictAction action = promptUserConflict(filename);

                    if (action == ConflictAction::Skip) {
                        std::cout << "Skipped: " << filename << std::endl;
                        continue;
                    } 
                    else if (action == ConflictAction::Rename) {
                        destPath = getUniquePath(destPath);
                    } 
                    // If Overwrite, keeps destPath
                }

                if (dryRun) {
                    std::cout << "[DRY-RUN] Would move: " << filename << " -> " << destPath << std::endl;
                } else {
                    std::filesystem::rename(entry.path(), destPath);
                    std::cout << "Moved: " << filename << " -> " << destPath << std::endl;
                }
            }
        }
    }
}

} // namespace organise

#include "../include/organiser.hpp"
#include <iostream>
#include <cctype>

namespace organise {

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

static ConflictMode promptUserConflict(const std::filesystem::path& filename) {
    while (true) {
        std::cout << "The file '" << filename.string() << "' Already exists.\n"
                  << "  [r]ename (as " << filename.stem().string() << " (1)" << filename.extension().string() << ")\n"
                  << "  [s]kip\n"
                  << "  [o]verwrite\n"
                  << "Choose [r/s/o]: ";

        char choice;
        std::cin >> choice;
        choice = static_cast<char>(std::tolower(choice));

        if (choice == 'r') return ConflictMode::Rename;
        if (choice == 's') return ConflictMode::Skip;
        if (choice == 'o') return ConflictMode::Overwrite;

        std::cout << "Invalid option! Try again.\n\n";
    }
}

static void handleFile(const std::filesystem::directory_entry& entry, const nlohmann::json& rules, const Options& opts) {
    auto ext = entry.path().extension();
    auto filename = entry.path().filename();

    auto it = rules.find(ext);
    if (it == rules.end()) {
        if (opts.verbose) {
            std::cout << "[VERBOSE] No rule for the extension: " << ext << " (" << filename << ")\n";
        }
        return;
    }

    std::filesystem::path destDir = it.value().get<std::filesystem::path>();

    if (!opts.dryRun && !std::filesystem::exists(destDir)) {
        std::filesystem::create_directories(destDir);
    }

    std::filesystem::path destPath = destDir / filename;

    if (std::filesystem::exists(destPath)) {
        if (opts.dryRun) {
            std::cout << "[DRY-RUN] Conflit detected for: " << filename << "\n";
            return;
        }

        ConflictMode action = opts.conflictMode;
        if (action == ConflictMode::Interactive) {
            action = promptUserConflict(filename);
        }

        if (action == ConflictMode::Skip) {
            std::cout << "Skipped: " << filename << "\n";
            return;
        } else if (action == ConflictMode::Rename) {
            destPath = getUniquePath(destPath);
        }
    }

    if (opts.dryRun) {
        std::cout << "[DRY-RUN] Would move: " << filename << " -> " << destPath << "\n";
    } else {
        std::filesystem::rename(entry.path(), destPath);
        std::cout << "Moved: " << filename << " -> " << destPath << "\n";
    }
}

void processDirectory(const nlohmann::json& rules, const Options& opts) {
    if (!std::filesystem::exists(opts.targetDir) || !std::filesystem::is_directory(opts.targetDir)) {
        std::cerr << "Error: Invalid directory " << opts.targetDir << std::endl;
        return;
    }

    if (opts.recursive) {
        for (const auto& entry : std::filesystem::recursive_directory_iterator(opts.targetDir)) {
            if (entry.is_regular_file()) {
                handleFile(entry, rules, opts);
            }
        }
    } else {
        for (const auto& entry : std::filesystem::directory_iterator(opts.targetDir)) {
            if (entry.is_regular_file()) {
                handleFile(entry, rules, opts);
            }
        }
    }
}

} // namespace organise

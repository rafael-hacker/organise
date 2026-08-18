#include "../include/organiser.hpp"
#include "../include/history.hpp"
#include "../include/colors.hpp"
#include <iostream>
#include <cctype>
#include <algorithm> // Needed for std::transform
#include <regex>

using namespace organise;
namespace organise {

static std::filesystem::path getUniquePath(const std::filesystem::path& destPath) {
    auto parent = destPath.parent_path();
    auto stem = destPath.stem().string();
    auto ext = destPath.extension().string(); // Gets the extension from destPath
    
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
        std::cout << color::yellow << "The file '" << filename.string() << "' Already exists.\n" << color::reset << color::blue
                  << "  [r]ename (as " << filename.stem().string() << " (1)" << filename.extension().string() << ")\n"
                  << "  [s]kip\n"
                  << "  [o]verwrite\n" << color::reset
                  << "Choose [r/s/o]: ";

        char choice;
        std::cin >> choice;
        choice = static_cast<char>(std::tolower(choice));

        if (choice == 'r') return ConflictMode::Rename;
        if (choice == 's') return ConflictMode::Skip;
        if (choice == 'o') return ConflictMode::Overwrite;

        std::cout << color::red << "Invalid option! Try again.\n\n" << color::reset;
    }
}

static void handleFile(const std::filesystem::directory_entry& entry, const nlohmann::json& rules, const Options& opts) {
    auto filename = entry.path().filename().string();
    auto ext = entry.path().extension().string();
    std::transform(ext.begin(), ext.end(), ext.begin(), [](unsigned char c){ return std::tolower(c); });

    std::filesystem::path destDir;
    bool found = false;

    for (auto& [pattern, path] : rules.items()) {
        // If it is regex (ex: ".*\\.pdf" ou "^Work_.*")
        std::regex re(pattern);
        if (std::regex_match(filename, re) || (pattern == ext)) {
            destDir = path.get<std::filesystem::path>();
            found = true;
            break;
        }
    }

    if (!found) return;

    // (removed the duplicate/broken `destDir` redeclaration that referenced `it`)

    if (!opts.dryRun && !std::filesystem::exists(destDir)) {
        std::filesystem::create_directories(destDir);
    }

    if (!opts.dryRun && !std::filesystem::exists(destDir)) {
        std::filesystem::create_directories(destDir);
    }
    
    std::filesystem::path destPath = destDir / filename;
    
    if (std::filesystem::exists(destPath)) {
        if (opts.dryRun) {
            std::cout << color::yellow << "[DRY-RUN] Conflict detected for: " << filename << color::reset << std::endl;
            return;
        }

        ConflictMode action = opts.conflictMode;
        if (action == ConflictMode::Interactive) {
            action = promptUserConflict(filename);
        }

        if (action == ConflictMode::Skip) {
            std::cout << color::yellow << "Skipped: " << filename << color::reset << std::endl;
            return;
        } else if (action == ConflictMode::Rename) {
            destPath = getUniquePath(destPath);
        }
    }

    if (opts.dryRun) {
        std::cout << color::yellow << "[DRY-RUN] Would move: " << filename << " -> " << destPath.string() << color::reset << std::endl;
    } else {
        try {
            std::filesystem::rename(entry.path(), destPath);
            std::cout << color::yellow << "Moved: " << filename << " -> " << destPath.string() << color::reset << std::endl;
            history::logMove(entry.path(), destPath);
        } catch (const std::filesystem::filesystem_error& e) {
            if (e.code() == std::errc::cross_device_link) {
                std::filesystem::copy(entry.path(), destPath, std::filesystem::copy_options::overwrite_existing);
                std::filesystem::remove(entry.path());
                std::cout << color::yellow << "Moved (Cross-device): " << filename << " -> " << destPath.string() << color::reset << std::endl;
                history::logMove(entry.path(), destPath);
            } else {
                std::cerr << color::red << "Error moving " << filename << ": " << e.what() << color::reset << std::endl;
            }
        }
    }
}

void processDirectory(const nlohmann::json& rules, const Options& opts) {
    if (!std::filesystem::exists(opts.targetDir) || !std::filesystem::is_directory(opts.targetDir)) {
        std::cerr << color::red << "Error: Invalid directory " << opts.targetDir.string() << color::reset << std::endl;
        return;
    }

    if (opts.recursive) {
        auto options = std::filesystem::directory_options::skip_permission_denied;
        for (const auto& entry : std::filesystem::recursive_directory_iterator(opts.targetDir, options)) {
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
}

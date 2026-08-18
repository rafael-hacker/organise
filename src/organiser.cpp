#include "organise/organiser.hpp"
#include "organise/history.hpp"
#include "organise/colors.hpp"
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

// Forward declaration, defined near processDirectory below.
static bool isSubPath(const std::filesystem::path& base, const std::filesystem::path& candidate);

static ConflictMode promptUserConflict(const std::filesystem::path& filename) {
    while (true) {
        std::cout << color::yellow << "The file '" << filename.string() << "' Already exists.\n" << color::reset << color::blue
                  << "  [r]ename (as " << filename.stem().string() << " (1)" << filename.extension().string() << ")\n"
                  << "  [s]kip\n"
                  << "  [o]verwrite\n" << color::reset
                  << "Choose [r/s/o]: ";

	std::string input;
	std::getline(std::cin, input);

	if (input.empty()) continue;
	char choice = static_cast<char>(std::tolower(input[0]));

        if (choice == 'r') return ConflictMode::Rename;
        if (choice == 's') return ConflictMode::Skip;
        if (choice == 'o') return ConflictMode::Overwrite;

        std::cout << color::red << "Invalid option! Try again.\n\n" << color::reset;
    }
}

void handleFile(const std::filesystem::directory_entry& entry, const nlohmann::json& rules, const Options& opts) {
    auto filename = entry.path().filename().string();
    auto ext = entry.path().extension().string();
    std::transform(ext.begin(), ext.end(), ext.begin(), [](unsigned char c){ return std::tolower(c); });

    std::filesystem::path destDir;
    bool found = false;
    std::string matchedPattern;

    for (auto& [pattern, path] : rules.items()) {
	if (pattern == ext) {
            destDir = path.get<std::filesystem::path>();
            found = true;
            matchedPattern = pattern;
            break;
        }

        try {
            std::regex re(pattern);
            if (std::regex_match(filename, re)) {
                destDir = path.get<std::filesystem::path>();
                found = true;
                matchedPattern = pattern;
                break;
            }
        } catch (const std::regex_error&) {
		/* No need for nothing here */
        }
    }

    if (!found) {
        if (opts.verbose) {
            std::cout << color::cyan << "[VERBOSE] No matching rule for '" << filename << "', leaving in place." << color::reset << std::endl;
        }
        return;
    }

    if (opts.verbose) {
        std::cout << color::cyan << "[VERBOSE] '" << filename << "' matched rule '" << matchedPattern
                   << "' -> " << destDir.string() << color::reset << std::endl;
    }

    if (opts.recursive && isSubPath(opts.targetDir, destDir)) {
        std::cerr << color::red << "Skipped " << filename
                   << ": destination '" << destDir.string()
                   << "' is inside the recursively-scanned directory, which could cause repeated re-organising."
                   << color::reset << std::endl;
        return;
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

// True if candidate is inside (or equal to) base, comparing canonical paths.
// Used to stop a config rule from sending files into a directory that's part
// of the very tree we're scanning -- which would otherwise cause moved files
// to be picked up and re-organised again on a later step of the same scan.
static bool isSubPath(const std::filesystem::path& base, const std::filesystem::path& candidate) {
    std::error_code ec;
    auto canonBase = std::filesystem::weakly_canonical(base, ec);
    if (ec) return false;
    auto canonCandidate = std::filesystem::weakly_canonical(candidate, ec);
    if (ec) return false;

    auto baseIt = canonBase.begin();
    auto candIt = canonCandidate.begin();
    for (; baseIt != canonBase.end(); ++baseIt, ++candIt) {
        if (candIt == canonCandidate.end() || *candIt != *baseIt) return false;
    }
    return true;
}

void processDirectory(const nlohmann::json& rules, const Options& opts) {
    if (!std::filesystem::exists(opts.targetDir) || !std::filesystem::is_directory(opts.targetDir)) {
        std::cerr << color::red << "Error: Invalid directory " << opts.targetDir.string() << color::reset << std::endl;
        return;
    }

    // Collect the full file list BEFORE moving anything. Moving files while a
    // std::filesystem::recursive_directory_iterator is actively walking the same tree is undefined behavior
    std::vector<std::filesystem::path> files;
    if (opts.recursive) {
        auto dirOpts = std::filesystem::directory_options::skip_permission_denied;
        for (const auto& entry : std::filesystem::recursive_directory_iterator(opts.targetDir, dirOpts)) {
            if (entry.is_regular_file()) files.push_back(entry.path());
        }
    } else {
        for (const auto& entry : std::filesystem::directory_iterator(opts.targetDir)) {
            if (entry.is_regular_file()) files.push_back(entry.path());
        }
    }

    if (opts.verbose) {
        std::cout << color::cyan << "[VERBOSE] Scanning " << opts.targetDir.string()
                   << (opts.recursive ? " (recursive)" : "") << " -- found " << files.size()
                   << " file(s) to check." << color::reset << std::endl;
    }

    for (const auto& filePath : files) {
        // The file may have already been moved by an earlier iteration (e.g.
        // duplicate rules), or removed externally -- skip if it's gone.
        if (!std::filesystem::exists(filePath)) continue;
        handleFile(std::filesystem::directory_entry(filePath), rules, opts);
    }
}
}

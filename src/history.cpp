#include "organise/history.hpp"
#include "organise/colors.hpp"
#include <nlohmann/json.hpp>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <cstdlib>

namespace organise::history {

static std::filesystem::path getHistoryPath() {
    std::filesystem::path baseDir;
    const char* xdgState = std::getenv("XDG_STATE_HOME");
    
    if (xdgState && xdgState[0] != '\0') {
        baseDir = std::filesystem::path(xdgState);
    } else {
        const char* home = std::getenv("HOME");
        baseDir = home ? std::filesystem::path(home) / ".local" / "state" : std::filesystem::current_path();
    }
    
    std::filesystem::path dir = baseDir / "organise";
    if (!std::filesystem::exists(dir)) {
        std::filesystem::create_directories(dir);
    }
    return dir / "history.json";
}

void logMove(const std::filesystem::path& original, const std::filesystem::path& dest) {
    auto path = getHistoryPath();
    nlohmann::json j = nlohmann::json::array();
    
    if (std::filesystem::exists(path)) {
        std::ifstream inFile(path);
        if (inFile.is_open()) {
            inFile >> j;
        }
    }
    
    j.push_back({{"original", original.string()}, {"destination", dest.string()}});
    
    std::ofstream outFile(path);
    outFile << j.dump(4);
}

void undo() {
    auto path = getHistoryPath();
    if (!std::filesystem::exists(path)) {
        std::cout << xcolor::yellow << "No history found, nothing to undo.\n" << xcolor::reset;
        return;
    }

    std::ifstream inFile(path);
    nlohmann::json j;
    inFile >> j;
    inFile.close();

    if (j.empty()) {
        std::cout << xcolor::yellow << "The history is empty.\n" << xcolor::reset;
        return;
    }

    std::cout << xcolor::blue << "Reversing " << j.size() << " changes...\n" << xcolor::reset;

    for (auto it = j.rbegin(); it != j.rend(); ++it) {
        std::filesystem::path orig = (*it)["original"];
        std::filesystem::path dest = (*it)["destination"];

        if (std::filesystem::exists(dest)) {
            try {
                std::filesystem::create_directories(orig.parent_path());
                std::filesystem::rename(dest, orig);
                std::cout << xcolor::green << "Undone: " << dest.filename().string() << " -> " << orig.string() << xcolor::reset << "\n";
            } catch (const std::exception& e) {
                std::cerr << xcolor::red << "Error undoing " << dest.filename().string() << ": " << e.what() << xcolor::reset << "\n";
            }
        }
    }
    
    std::filesystem::remove(path);
    std::cout << xcolor::blue << "Clean history.\n" << xcolor::reset;
}

} // namespace organise::history

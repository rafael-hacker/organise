#include "organise/history.hpp"
#include "organise/log.hpp"
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
        std::cout << color::yellow << "No history found, nothing to undo.\n" << color::reset;
        return;
    }

    std::ifstream inFile(path);
    nlohmann::json j;
    inFile >> j;
    inFile.close();

    if (j.empty()) {
        std::cout << color::yellow << "The history is empty.\n" << color::reset;
        return;
    }

    std::cout << color::blue << "Reversing " << j.size() << " changes...\n" << color::reset;

    for (auto it = j.rbegin(); it != j.rend(); ++it) {
        std::filesystem::path orig = (*it)["original"];
        std::filesystem::path dest = (*it)["destination"];

        if (std::filesystem::exists(dest)) {
            try {
                std::filesystem::create_directories(orig.parent_path());
                std::filesystem::rename(dest, orig);
                std::cout << color::green << "Undone: " << dest.filename().string() << " -> " << orig.string() << color::reset << "\n";
		log::logActivity("Undid move: " + dest.filename().string() + " → " + orig.string());
            } catch (const std::exception& e) {
                std::cerr << color::red << "Error undoing " << dest.filename().string() << ": " << e.what() << color::reset << "\n";
		log::logActivity("ERROR undoing " + dest.filename().string() + ": " + e.what());
            }
        }
    }
    
    std::filesystem::remove(path);
    std::cout << color::blue << "Clean history.\n" << color::reset;
}

} // namespace organise::history

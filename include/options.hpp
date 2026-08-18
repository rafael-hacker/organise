#pragma once

#include <filesystem>
#include <string>

namespace organise {

enum class ConflictMode {
    Interactive,
    Rename,
    Skip,
    Overwrite
};

struct Options {
    std::filesystem::path targetDir;
    std::filesystem::path configPath;
    bool dryRun = false;
    bool showHelp = false;
    bool showVersion = false;
    bool recursive = false;
    bool undo = false;
    bool verbose = false;
    ConflictMode conflictMode = ConflictMode::Interactive;
};

Options parseArgs(int argc, char* argv[]);

void printHelp();

} // namespace organise

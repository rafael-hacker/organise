#pragma once

#include <filesystem>
#include <string>

namespace organise {

struct Options {
    std::filesystem::path targetDir;
    std::filesystem::path configPath;
    bool dryRun = false;
    bool showHelp = false;
    bool showVersion = false;
};

Options parseArgs(int argc, char* argv[]);

void printHelp();

} // namespace organise

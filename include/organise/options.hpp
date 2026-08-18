#pragma once

#include <getopt.h>
#include <unistd.h>
#include <filesystem>
#include <string>

namespace organise {

inline const struct option long_option[] = {
    {"dry-run", required_argument, nullptr, 'n'},
    {"recursive", required_argument, nullptr, 'r'},
    {"verbose", no_argument, nullptr, 'v'},
    {"path", required_argument, nullptr, 'p'},
    {"conflict", required_argument ,nullptr, 0},
    {"config", required_argument, nullptr, 'c'},
    {"auto-rename", no_argument, nullptr, 'y'},
    {"watch", required_argument, nullptr, 0},
    {"undo", no_argument, nullptr, 'u'},
    {"help", no_argument, nullptr, 'h'},
    {"version", no_argument, nullptr, 0},
    {nullptr, 0, nullptr, 0}
};

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
    bool watch = false;
    bool verbose = false;
    ConflictMode conflictMode = ConflictMode::Interactive;
};

Options parseArgs(int argc, char* argv[]);

void printHelp();

} // namespace organise

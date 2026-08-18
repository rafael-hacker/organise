#include "../include/options.hpp"
#include "../include/config.hpp"
#include <iostream>
#include <string_view>

namespace organise {

void printHelp() {
    std::cout << "\033[31m"<< "Usage: org [OPTIONS] <path>\n\n" << ""
              << "\033[35m"<<"Options:\n"
              << "\033[30m"<<"  -n, --dry-run          Simulate actions without moving files\n" << "\033[0m"
              << "\033[30m"<<"  -r, --recursive        Scan directories recursively\n" << "\033[0m"
              << "\033[30m"<<"  -v, --verbose          Display detailed execution output\n" << "\033[0m"
              << "\033[30m"<<"  -y, --auto-rename      Automatically rename conflicting files (no prompt)\n" << "\033[0m"
              << "\033[30m"<<"      --conflict <mode>  Set conflict strategy: rename, skip, or overwrite\n" << "\033[0m"
              << "\033[30m"<<"  -c, --config <path>    Custom configuration file path\n" << "\033[0m"
              << "\033[32m"<<"  -h, --help             Display this help message\n " << "\033[0m"
              << "\033[34m" <<"      --version          Display version information\n" << "\033[0m";
}

Options parseArgs(int argc, char* argv[]) {
    Options opts;

    for (int i = 1; i < argc; ++i) {
        std::string_view arg = argv[i];

        if (arg == "-h" || arg == "--help") {
            opts.showHelp = true;
        } else if (arg == "--version") {
            opts.showVersion = true;
        } else if (arg == "-n" || arg == "--dry-run") {
            opts.dryRun = true;
        } else if (arg == "-r" || arg == "--recursive") {
            opts.recursive = true;
        } else if (arg == "-v" || arg == "--verbose") {
            opts.verbose = true;
        } else if (arg == "-y" || arg == "--auto-rename") {
            opts.conflictMode = ConflictMode::Rename;
        } else if (arg == "--conflict" && i + 1 < argc) {
            std::string_view mode = argv[++i];
            if (mode == "skip") opts.conflictMode = ConflictMode::Skip;
            else if (mode == "overwrite") opts.conflictMode = ConflictMode::Overwrite;
            else opts.conflictMode = ConflictMode::Rename;
        } else if ((arg == "-c" || arg == "--config") && i + 1 < argc) {
            opts.configPath = argv[++i];
        } else if (arg[0] != '-') {
            opts.targetDir = arg;
        }
    }

    if (opts.configPath.empty()) {
        opts.configPath = getDefaultConfigPath();
    }

    return opts;
}

} // namespace organise

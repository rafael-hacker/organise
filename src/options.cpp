#include "../include/options.hpp"
#include "../include/config.hpp"
#include <iostream>
#include <string_view>

namespace organise {

void printHelp() {
    std::cout << "Usage: org [OPTIONS] <path>\n\n"
              << "Options:\n"
              << "  -n, --dry-run          Simulate actions without moving files\n"
              << "  -r, --recursive        Scan directories recursively\n"
              << "  -v, --verbose          Display detailed execution output\n"
              << "  -y, --auto-rename      Automatically rename conflicting files (no prompt)\n"
              << "      --conflict <mode>  Set conflict strategy: rename, skip, or overwrite\n"
              << "  -c, --config <path>    Custom configuration file path\n"
              << "  -w, --watch            Watch directory continuously and organise new files as they appear\n"
              << "  -u, --undo             Undo the last batch of moves\n"
              << "  -h, --help             Display this help message\n"
              << "      --version          Display version information\n";
}

Options parseArgs(int argc, char* argv[]) {
    Options opts;

    for (int i = 1; i < argc; ++i) {
        std::string_view arg = argv[i];
        
        std::string arg_lower{arg};
        std::transform(arg_lower.begin(), arg_lower.end(), arg_lower.begin(), [](unsigned char c){
            return std::tolower(c);
        });

        if (arg_lower == "-h" || arg_lower == "--help") {
            opts.showHelp = true;
        } else if (arg_lower == "--version") {
            opts.showVersion = true;
        } else if (arg_lower == "-n" || arg_lower == "--dry-run") {
            opts.dryRun = true;
        } else if (arg_lower == "-r" || arg_lower == "--recursive") {
            opts.recursive = true;
        } else if (arg_lower == "-u" || arg_lower == "--undo") {
            opts.undo = true;
        } else if (arg_lower == "--watch" || arg_lower == "-w") {
            opts.watch = true;
            continue;
        } else if (arg_lower == "-v" || arg_lower == "--verbose") {
            opts.verbose = true;
        } else if (arg_lower == "-y" || arg_lower == "--auto-rename") {
            opts.conflictMode = ConflictMode::Rename;
        } else if (arg == "--conflict" && i + 1 < argc) {
            std::string_view mode = argv[++i];
            if (mode == "skip") opts.conflictMode = ConflictMode::Skip;
            else if (mode == "overwrite") opts.conflictMode = ConflictMode::Overwrite;
            else opts.conflictMode = ConflictMode::Rename;
        } else if ((arg_lower == "-c" || arg_lower == "--config") && i + 1 < argc) {
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

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
              << "      --watch            Watch directory continuously and organise new files as they appear\n"
              << "  -u, --undo             Undo the last batch of moves\n"
              << "  -h, --help             Display this help message\n"
              << "      --version          Display version information\n";
}

Options parseArgs(int argc, char* argv[]) {
    Options opts;

    for (int i = 1; i < argc; ++i) {
        std::string_view arg = argv[i];

        if (arg.empty()) {
            continue;
        }

        if (arg == "-h" || arg == "--help") {
            opts.showHelp = true;
        } else if (arg == "--version") {
            opts.showVersion = true;
        } else if (arg == "-n" || arg == "--dry-run") {
            opts.dryRun = true;
        } else if (arg == "-r" || arg == "--recursive") {
            opts.recursive = true;
        } else if (arg == "-u" || arg == "--undo") {
            opts.undo = true;
        } else if (arg == "--watch") {
            opts.watch = true;
            continue;
        } else if (arg == "-v" || arg == "--verbose") {
            opts.verbose = true;
        } else if (arg == "-y" || arg == "--auto-rename") {
            opts.conflictMode = ConflictMode::Rename;
        } else if (arg == "--conflict") {
            if (i + 1 >= argc) {
                std::cerr << "Error: --conflict requires an argument (rename, skip, or overwrite).\n";
                continue;
            }
            std::string_view mode = argv[++i];
            if (mode == "skip") opts.conflictMode = ConflictMode::Skip;
            else if (mode == "overwrite") opts.conflictMode = ConflictMode::Overwrite;
            else if (mode == "rename") opts.conflictMode = ConflictMode::Rename;
            else std::cerr << "Warning: unrecognized --conflict mode '" << mode << "', ignoring.\n";
        } else if (arg == "-c" || arg == "--config") {
            if (i + 1 >= argc) {
                std::cerr << color::red << "Error: " << arg << " requires a path argument." << color::reset << std::endl;
                continue;
            }
            opts.configPath = argv[++i];
        } else if (arg[0] != '-') {
            opts.targetDir = arg;
        } else {
            std::cerr << color::yellow << "Warning: unrecognized option '" << arg << "', ignoring. Run with --help to see valid options." << color::reset << std::endl;
        }
    }

    if (opts.configPath.empty()) {
        opts.configPath = getDefaultConfigPath();
    }

    return opts;
}

} // namespace organise

#include "../include/options.hpp"
#include "../include/config.hpp"
#include <iostream>
#include <string_view>

namespace organise {

void printHelp() {
    std::cout << "Usage: org [OPTIONS] <path>\n\n"
              << "Options:\n"
              << "  -n, --dry-run     Simulate actions without actually doing them\n"
              << "  -c, --config PATH Custom configuration file path\n"
              << "  -h, --help        Display this help message\n"
              << "  -v, --version     Display version information\n";
}

Options parseArgs(int argc, char* argv[]) {
    Options opts;

    for (int i = 1; i < argc; ++i) {
        std::string_view arg = argv[i];

        if (arg == "-h" || arg == "--help") {
            opts.showHelp = true;
        } else if (arg == "-v" || arg == "--version") {
            opts.showVersion = true;
        } else if (arg == "-n" || arg == "--dry-run") {
            opts.dryRun = true;
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

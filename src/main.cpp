#include "../include/config.hpp"
#include "../include/options.hpp"
#include "../include/organiser.hpp"
#include <iostream>

int main(int argc, char* argv[]) {
    auto opts = organise::parseArgs(argc, argv);

    if (opts.showHelp) {
        organise::printHelp();
        return 0;
    }

    if (opts.showVersion) {
        std::cout << "org version 1.0.0\n";
        return 0;
    }

    if (opts.targetDir.empty()) {
        std::cerr << "Error: Target directory path is required.\n\n";
        organise::printHelp();
        return 1;
    }

    auto rules = organise::loadConfig(opts.configPath);
    if (rules.is_null()) {
        return 1;
    }

    organise::processDirectory(opts.targetDir, rules, opts.dryRun);
    return 0;
}

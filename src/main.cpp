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
        std::cout << "\033[34m" << "org version 1.0.0\n" << "\033[0m" ;
        return 0;
    }

    if (opts.targetDir.empty()) {
        std::cerr << "\033[31m" << "Error: Target directory is needed.\n\n" << "\033[0m";
        organise::printHelp();
        return 1;
    }

    auto rules = organise::loadConfig(opts.configPath);
    if (rules.is_null()) {
        return 1;
    }

    organise::processDirectory(rules, opts);
    return 0;
}

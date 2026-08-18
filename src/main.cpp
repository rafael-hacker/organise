#include "../include/config.hpp"
#include "../include/options.hpp"
#include "../include/organiser.hpp"
#include "../include/colors.hpp"
#include <iostream>

int main(int argc, char* argv[]) {
    auto opts = organise::parseArgs(argc, argv);

    if (opts.showHelp) {
        organise::printHelp();
        return 0;
    }

    if (opts.showVersion) {
        std::cout << color::blue << "org version 1.0.0" << color::reset << std::endl;
        return 0;
    }

    if (opts.targetDir.empty()) {
        std::cerr << color:red << "Error: Target directory is needed." << color::reset << std::endl;
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

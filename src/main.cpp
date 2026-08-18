#include "organise/config.hpp"
#include "organise/options.hpp"
#include "organise/organiser.hpp"
#include "organise/history.hpp"
#include "organise/colors.hpp"
#include "organise/version.hpp"
#include <iostream>

using namespace organise;
int main(int argc, char* argv[]) {
    auto opts = organise::parseArgs(argc, argv);

    if (opts.showHelp) {
        organise::printHelp();
        return 0;
    }

    if (opts.showVersion) {
        std::cout << color::blue << "org version " << ORGANISE_VERSION << color::reset << std::endl;
        return 0;
    }

    if (opts.targetDir.empty()) {
        std::cerr << color::red << "Error: Target directory is needed." << color::reset << std::endl;
        organise::printHelp();
        return 1;
    }

    if (opts.undo) {
        organise::history::undo();
        return 0;
    }

    auto rules = organise::loadConfig(opts.configPath);
    if (rules.is_null()) {
        return 1;
    }
    if (opts.watch) {
        startWatcher(rules, opts);
    } else {
        processDirectory(rules, opts);
    }
    return 0;
}

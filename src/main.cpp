#include "organise/config.hpp"
#include "organise/options.hpp"
#include "organise/organiser.hpp"
#include "organise/history.hpp"
#include "organise/colors.hpp"
#include "organise/version.hpp"
#include <iostream>
#include <ftxui/dom/elements.hpp>  // for operator|, text, Element, Fit, borderDouble, borderHeavy, borderLight, borderRounded, vbox
#include <ftxui/screen/screen.hpp>  // for Screen

using namespace ftxui;
using namespace organise;

int main(int argc, char* argv[]) {
    auto opts = organise::parseArgs(argc, argv);

    if (opts.showHelp) {
        organise::printHelp();
        return 0;
    }

    if (opts.showVersion) {
        Element document = hbox({
            text("󰏖 Organiser Version ") | color(Color::Cyan),
            text(ORGANISE_VERSION) | color(Color::Yellow)
        }) | borderStyled(ROUNDED, Color::GrayLight);
        auto screen = Screen::Create(
            Dimension::Fit(document)
        );
        Render(screen, document);
        screen.Print();
        std::cout << std::endl;
        return 0;
    }

    if (opts.targetDir.empty()) {
        std::cerr << xcolor::red << "Error: Target directory is needed." << xcolor::reset << std::endl;
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

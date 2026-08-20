#include "organise/options.hpp"
#include "organise/config.hpp"
#include "organise/colors.hpp"
#include <iostream>
#include <string_view>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>

namespace organise {

void printHelp() {
    using namespace ftxui;
    Element document = vbox({
        text("󰉏  Organiser ") | color(Color::Red),
        separator(),
        text(" 󰘳 Usage: 󰆍 org <command/flags> [path]\n\n") | color(Color::Red),
        text(" 󰞷 Options:\n") | color(Color::Purple),
        text(" 󰓛  -n, --dry-run          Simulate actions without moving files\n") | color(Color::Green),
        text(" 󰡁 -r, --recursive        Scan directories recursively\n") | color(Color::Green),
        text(" 󰐊  -v, --verbose          Display detailed execution output\n") | color(Color::Green),
        text(" 󰑕  -y, --auto-rename      Automatically rename conflicting files (no prompt)\n" ) | color(Color::Green),
        text(" 󰅚    --conflict <mode>  Set conflict strategy: rename, skip, or overwrite\n") | color(Color::Green),
        text(" 󰅩 -c, --config <path>    Custom configuration file path\n" ) | color(Color::Green),
        text(" 󰕏 -w, --watch            Watch directory continuously and organise new files as they appear\n" ) | color(Color::Green),
        text(" 󰕌 -u, --undo             Undo the last batch of moves\n") | color(Color::Green),
        text(" 󰞷 -h, --help             Display this help message\n" ) | color(Color::Cyan),
        text(" 󰏖    --version          Display version information\n" ) | color(Color::Blue)
}) | borderStyled(ROUNDED, Color::GrayLight);
    
    auto screen = Screen::Create(
    Dimension::Fit(document) // Height
    );
    Render(screen, document);
 
    // Print the screen to the console.
    screen.Print();
    std::cout<<std::endl;
}

Options parseArgs(int argc, char* argv[]) {
    Options opts;
    int opt;
    int opt_index = 0;
    optind = 1;
    opterr = 0;
    while((opt = getopt_long(argc , argv, "nrvyc:p:uh", long_option,&opt_index)) != -1){
        if (opt == 0){
            std::string arg = long_option[opt_index].name;
             if (arg == "help") {
                opts.showHelp = true;
            } else if (arg == "version") {
                opts.showVersion = true;
            } else if (arg == "dry-run") {
                opts.dryRun = true;
            } else if (arg == "recursive") {
                opts.recursive = true;
            } else if (arg == "undo") {
                opts.undo = true;
            } else if (arg == "watch") {
                opts.watch = true;
                continue;
            } else if (arg == "verbose") {
                opts.verbose = true;
            } else if (arg == "auto-rename") {
                opts.conflictMode = ConflictMode::Rename;
            } else if (arg == "conflict") {
                std::string_view mode = optarg;             
                
                if (mode == "skip") opts.conflictMode = ConflictMode::Skip;
                else if (mode == "overwrite") opts.conflictMode = ConflictMode::Overwrite;
                else if (mode == "rename") opts.conflictMode = ConflictMode::Rename;
                else std::cerr << xcolor::yellow << "Warning: unrecognized --conflict mode '" << mode << "', ignoring." << xcolor::reset << std::endl;
            
            } else if (arg == "config") {
                opts.configPath = optarg;
            } else if (arg == "path") {
                opts.targetDir = optarg;
            }
            continue;
        }
        
        switch (opt){
            case 'p': opts.targetDir = optarg; break;
            case 'h': opts.showHelp = true; break;
            case 'n': opts.dryRun = true; break;
            case 'r': opts.recursive = true; break;
            case 'u': opts.undo = true; break;
            case 'v': opts.verbose = true; break;
            case 'y': opts.conflictMode = ConflictMode::Rename; break; 
            case 'c': opts.configPath = optarg; break;                 
            
            case '?': {
                if (optopt == 'c'){
                     std::cerr << xcolor::red << "Error: -c requires a path argument." << xcolor::reset << std::endl;
                } else if ((std::string_view(argv[optind - 1]).find("--conflict") != std::string_view::npos)) {
                    std::cerr << xcolor::red << "Error: --conflict requires an argument (rename, skip, or overwrite)." << xcolor::reset << std::endl;
                } else {
                     std::cerr << xcolor::yellow << "Warning: unrecognized option '" << argv[optind - 1] 
                              << "', ignoring. Run with --help to see valid options." << xcolor::reset << std::endl;
                }
                break;
            }
            default:
                break;
        }
    }

    if (opts.configPath.empty()) {
        opts.configPath = getDefaultConfigPath();
    }

    return opts;
}

} // namespace organise

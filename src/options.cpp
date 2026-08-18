#include "../include/options.hpp"
#include "../include/config.hpp"
#include "../include/colors.hpp"
#include <iostream>
#include <string_view>

namespace organise {

void printHelp() {
    std::cout << color::red    <<"Usage: org [OPTIONS] <path>\n\n" << color::reset
              << color::purple << "Options:\n" << color::reset
              << color::green  <<"  -n, --dry-run          Simulate actions without moving files\n" << color::reset
              << color::green  <<"  -r, --recursive        Scan directories recursively\n" << color::reset
              << color::green  <<"  -v, --verbose          Display detailed execution output\n" << color::reset
              << color::green  <<"  -y, --auto-rename      Automatically rename conflicting files (no prompt)\n" << color::reset
              << color::green  <<"      --conflict <mode>  Set conflict strategy: rename, skip, or overwrite\n" << color::reset
              << color::green  <<"  -c, --config <path>    Custom configuration file path\n" << color::reset
              << color::green  <<"  -w, --watch            Watch directory continuously and organise new files as they appear\n" << color::reset
              << color::green  <<"  -u, --undo             Undo the last batch of moves\n" << color::reset
              << color::cyan   <<"  -h, --help             Display this help message\n" << color::reset
              << color::blue   <<"      --version          Display version information\n" << color::reset;
}

Options parseArgs(int argc, char* argv[]) {
    Options opts;
    int opt;
    int opt_index = 0;
    optind = 1;
    opterr = 0;
    while((opt = getopt_long(argc , argv, "n:r:vycp:uh", long_option,&opt_index)) != -1){
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
            } else if ("watch") {
                opts.watch = true;
                continue;
            } else if (arg == "verbose") {
                opts.verbose = true;
            } else if (arg == "auto-rename") {
                opts.conflictMode = ConflictMode::Rename;
            } else if ("conflict") {
                std::string_view mode = optarg;             
                
                if (mode == "skip") opts.conflictMode = ConflictMode::Skip;
                else if (mode == "overwrite") opts.conflictMode = ConflictMode::Overwrite;
                else if (mode == "rename") opts.conflictMode = ConflictMode::Rename;
                else std::cerr << color::yellow << "Warning: unrecognized --conflict mode '" << mode << "', ignoring." << color::reset << std::endl;
            
            } else if (arg == "config") {
                opts.configPath = optarg;
            } else if (arg == "path") {
                opts.targetDir = optarg;
            }
            continue;
        }
        
        switch (opt){
            case 'h': opts.showHelp = true; break;
            case 'n': opts.dryRun = true; break;
            case 'r': opts.recursive = true; break;
            case 'u': opts.undo = true; break;
            case 'v': opts.verbose = true; break;
            case 'y': opts.conflictMode = ConflictMode::Rename; break; 
            case 'c': opts.configPath = optarg; break;                 
            
            case '?': {
                if (optopt == 'c'){
                     std::cerr << color::red << "Error: -c requires a path argument." << color::reset << std::endl;
                } else if ((std::string_view(argv[optind - 1]).find("--conflict") != std::string_view::npos)) {
                    std::cerr << color::red << "Error: --conflict requires an argument (rename, skip, or overwrite)." << color::reset << std::endl;
                } else {
                     std::cerr << color::yellow << "Warning: unrecognized option '" << argv[optind - 1] 
                              << "', ignoring. Run with --help to see valid options." << color::reset << std::endl;
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

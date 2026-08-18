#include <filesystem>
#include <fstream>
#include <iostream>
#include <cstdlib> // For std::getenv
#include <nlohmann/json.hpp>

using json = nlohmann::json;

int main(int argc, char *argv[]){
    if (argc < 2){
        std::cout << "usage: org <path>" << std::endl;
        return 1;
    }
    

    const char* homeDir = std::getenv("HOME");
    if (!homeDir) {
        std::cerr << "Error: Environment variable HOME is not defined." << std::endl;
        return 1;
    }

    std::filesystem::path configPath = std::filesystem::path(homeDir) / ".config" / "organise" / "config.json";
    
    std::ifstream config(configPath);
    if (!config.is_open()){
        std::cerr << "Failed to read config.json. " << configPath << std::endl;
        std::cerr << "Make sure to create the file before running the program." << std::endl;
        return 1;
    }

    json data;
    config >> data; 
    
    for (auto& entry : std::filesystem::directory_iterator(argv[1])){
       if (entry.is_regular_file()){

            auto ext = entry.path().extension();
            auto filename = entry.path().filename();
            std::filesystem::path source = entry.path();
            auto it = data.find(ext);
            
            if (it != data.end()){
                std::filesystem::path path = it.value().get<std::filesystem::path>();
                
                if (!std::filesystem::exists(path)) {
                    std::filesystem::create_directories(path);
                }

                std::filesystem::path dest = path / filename;
                std::filesystem::rename(source, dest);
            }
       }
    }
    
    return 0;
}

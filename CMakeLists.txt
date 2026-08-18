cmake_minimum_required(VERSION 3.14)

if(POLICY CMP0135)
    cmake_policy(SET CMP0135 NEW)
endif()

project(organise VERSION 1.0.0 LANGUAGES CXX)

# C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# - Dependencies
include(FetchContent)
FetchContent_Declare(
    nlohmann_json
    URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
)
FetchContent_MakeAvailable(nlohmann_json)

# Executable

set(SOURCES
    src/main.cpp
    src/config.cpp
    src/options.cpp
    src/organiser.cpp
    src/history.cpp
    src/watcher.cpp
)

# Creates 'org' executable
add_executable(org ${SOURCES})

# Includes headers dirs
target_include_directories(org PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(org PRIVATE nlohmann_json::nlohmann_json)

# Installation
include(GNUInstallDirs)

install(TARGETS org
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

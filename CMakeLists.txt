cmake_minimum_required(VERSION 3.14)
project(organise VERSION 1.0.0 LANGUAGES CXX)

# C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ------------------------------------------------------------------------------
# - Dependencies (nlohmann/json via FetchContent)
# ------------------------------------------------------------------------------
include(FetchContent)

FetchContent_Declare(
    json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG        v3.11.3
)
FetchContent_MakeAvailable(json)

# ------------------------------------------------------------------------------
# Executable
# ------------------------------------------------------------------------------

set(SOURCES
    src/main.cpp
    src/config.cpp
    src/organiser.cpp
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

cmake_minimum_required(VERSION 3.14)

if(POLICY CMP0135)
    cmake_policy(SET CMP0135 NEW)
endif()

project(organise VERSION 1.0.0 LANGUAGES CXX)

# ---- C++17 ----
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Build type padrão (evita binário sem otimização por engano)
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# ---- Dependências ----
include(FetchContent)

# nlohmann/json
FetchContent_Declare(
    nlohmann_json
    URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
    URL_HASH SHA256=d6c65aca6b1ed68e7a182f4757257b107ae403032760ed6ef121c9d55e81757d
)

# FTXUI (TUI) — era isso que faltava
set(FTXUI_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(FTXUI_BUILD_DOCS     OFF CACHE BOOL "" FORCE)
set(FTXUI_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
set(FTXUI_ENABLE_INSTALL OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    ftxui
    GIT_REPOSITORY https://github.com/ArthurSonzogni/FTXUI.git
    GIT_TAG        v5.0.0
    GIT_SHALLOW    TRUE
)

FetchContent_MakeAvailable(nlohmann_json ftxui)

# Threads (FTXUI component usa std::thread)
find_package(Threads REQUIRED)

# ---- Geração do version.hpp ----
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/include/organise/version.hpp.in
    ${CMAKE_CURRENT_BINARY_DIR}/generated/organise/version.hpp
    @ONLY
)

# ---- Executável ----
set(SOURCES
    src/main.cpp
    src/config.cpp
    src/options.cpp
    src/organiser.cpp
    src/history.cpp
    src/watcher.cpp
)

add_executable(org ${SOURCES})

target_include_directories(org PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/generated
)

target_link_libraries(org PRIVATE
    nlohmann_json::nlohmann_json
    ftxui::screen
    ftxui::dom
    ftxui::component      # remova se não usar Component/interatividade
    Threads::Threads
)

# std::filesystem em GCC < 9
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 9.0)
    target_link_libraries(org PRIVATE stdc++fs)
endif()

# ---- Instalação ----
include(GNUInstallDirs)

install(TARGETS org RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

install(DIRECTORY include/organise
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    FILES_MATCHING PATTERN "*.hpp" PATTERN "*.h"
)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/generated/organise/version.hpp
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/organise
)

install(FILES man/org.1
    DESTINATION ${CMAKE_INSTALL_MANDIR}/man1)
install(FILES completions/org.bash
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/bash-completion/completions RENAME org)
install(FILES completions/org.zsh
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/zsh/site-functions RENAME _org)
install(FILES completions/org.fish
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/fish/vendor_completions.d)


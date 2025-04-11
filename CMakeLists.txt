cmake_minimum_required(VERSION 3.28)
project(cs2_replay_tool LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

#SFML library
include(FetchContent)
FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 3.0.0
        GIT_SHALLOW ON
        EXCLUDE_FROM_ALL
        SYSTEM)
FetchContent_MakeAvailable(SFML)

#Json management library
find_package(nlohmann_json CONFIG REQUIRED)

add_executable(cs2_replay_tool
        src/analysis/Player.hpp
        src/analysis/Player.cpp
        src/analysis/Round.hpp
        src/analysis/Round.cpp
        src/renderer/Render.cpp
        src/renderer/Render.hpp
        src/main.cpp)

target_link_libraries(cs2_replay_tool PRIVATE
        nlohmann_json::nlohmann_json
)
target_link_libraries(cs2_replay_tool PRIVATE
        SFML::System
        SFML::Window
        SFML::Graphics
        SFML::Network
        SFML::Audio
)
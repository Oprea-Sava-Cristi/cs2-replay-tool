cmake_minimum_required(VERSION 3.21)
project(cs2_replay_tool)

set(CMAKE_CXX_STANDARD 17)

find_package(nlohmann_json CONFIG REQUIRED)

add_executable(cs2_replay_tool
        src/analysis/test.cpp)
target_link_libraries(cs2_replay_tool PRIVATE nlohmann_json::nlohmann_json)

#include "renderer/Render.hpp"
#include "analysis/Round.hpp"
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

Round createRound() {

    std::string filePath = R"(.\data\player_data.json)";
    std::ifstream file(filePath);
    if (!file) {
        std::cerr << "Failed to open player_data.json" << std::endl;
        return {};
    }

    json j;
    file >> j;
    file.close();
    try {
        Round round(j);
        return round;
    } catch (const std::exception &ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
    }

}

int main() {
    Round round = createRound();
    renderMap(round);
    return 0;
}


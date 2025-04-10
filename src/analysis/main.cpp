#include <iostream>
#include <fstream>
#include "Demo.hpp"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

int main() {

    std::string filePath = R"(.\data\player_data.json)";
    std::ifstream file(filePath);
    if (!file) {
        std::cerr << "Failed to open player_data.json" << std::endl;
        return 1;
    }

    json j;
    file >> j;
    file.close();

    try {
        Demo demo(j);
        demo.print();
    } catch (const std::exception &ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
    }

    return 0;
}

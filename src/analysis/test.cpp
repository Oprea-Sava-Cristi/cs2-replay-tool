#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

int main() {
    json j;
    j["name"] = "Alice";
    j["age"] = 30;
    j["is_student"] = false;
    j["scores"] = {95, 87, 92};
    j["address"] = {
        {"street", "123 Main St"},
        {"city", "Wonderland"}
    };

    std::string serializedJson = j.dump(4);
    std::cout << "Serialized JSON:\n" << serializedJson << std::endl;

    return 0;
}
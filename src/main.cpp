#include "renderer/Render.hpp"
#include "analysis/Round.hpp"
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

int main() {
    Round round = createRound("");
    renderMap(round);
    return 0;
}


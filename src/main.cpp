#include "renderer/Render.hpp"
#include "analysis/Round.hpp"
#include <iostream>

int main() {
    Round round = createRound(R"(./data/player_data (1).json)");
    renderMap(round);
    return 0;
}


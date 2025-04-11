#include "renderer/Render.hpp"
#include "analysis/Round.hpp"
#include <iostream>

int main() {
    Round round = createRound("");
    renderMap(round);
    return 0;
}


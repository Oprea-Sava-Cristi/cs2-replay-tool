#include "Round.hpp"
#include <iostream>
#include <stdexcept>
#include <string>

Round::Round() {
    meta = {0, false, 0.0, "", 1.0, 0, 0, 0, 0, {0, 0}, 64, 0, 0, 0};
}

Round::Round(const json &j) {
    if (j.contains("meta")) {
        const json &m = j["meta"];
        meta.fromTick = m.value("fromTick", 0);
        meta.isTwoFloor = m.value("isTwoFloor", false);
        meta.lowerSectionBorderZ = m.value("lowerSectionBorderZ", 0.0);
        meta.mapName = m.value("mapName", "");
        meta.mapScale = m.value("mapScale", 1.0);
        meta.roundFreezeEndTick = m.value("roundFreezeEndTick", 0);
        meta.roundNumber = m.value("roundNumber", 0);
        meta.roundScoreUpdateTick = m.value("roundScoreUpdateTick", 0);
        meta.roundTime = m.value("roundTime", 0);
        meta.score = m.value("score", std::vector<int>{0, 0});
        meta.tickRate = m.value("tickRate", 64);
        meta.toTick = m.value("toTick", 0);
        meta.winReason = m.value("winReason", 0);
        meta.winnerTeam = m.value("winnerTeam", 0);
    } else {
        throw std::runtime_error("JSON does not contain \"meta\" field.");
    }

    if (j.contains("playerPositions")) {
        const json &players = j["playerPositions"];
        for (auto it = players.begin(); it != players.end(); ++it) {
            uint64_t steamid = std::stoull(it.key());
            const json &playerInfo = it.value();
            std::string name = playerInfo.value("name", "Unknown");

            if (playerInfo.contains("frames")) {
                const json &frames = playerInfo["frames"];
                Player p(steamid, name, frames);
                playerPositions[steamid] = p;
            }
        }
    } else {
        throw std::runtime_error("JSON does not contain \"playerPositions\" field.");
    }
}

void Round::print() const {
    std::cout << "Map: " << meta.mapName << "\n"
              << "Round: " << meta.roundNumber << "\n"
              << "Round Time: " << meta.roundTime << "\n"
              << "Score: " << meta.score[0] << " - " << meta.score[1] << "\n"
              << "--------------------------" << std::endl;

    for (const auto &entry : playerPositions) {
        entry.second.print();
    }
}

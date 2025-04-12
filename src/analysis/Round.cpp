#include "Round.hpp"
#include <iostream>
#include <stdexcept>
#include <string>
#include <fstream>

Round::Round() {
    meta = {
        0,                  // fromTick
        false,              // isTwoFloor
        0.0,                // lowerSectionBorderZ
        "",                 // mapName
        1.0,                // mapScale
        std::pair{0, 0},// mapOffset
        0,                  // roundFreezeEndTick
        0,                  // roundNumber
        0,                  // roundScoreUpdateTick
        0,                  // roundTime
        std::pair{0, 0},// score
        64,                 // tickRate
        0,                  // toTick
        0,                  // winReason
        0                   // winnerTeam
    };
}

Round::Round(const json &j) {
    if (j.contains("meta")) {
        const json &m = j["meta"];
        meta.fromTick = m.value("fromTick", 0);
        meta.isTwoFloor = m.value("isTwoFloor", false);
        meta.lowerSectionBorderZ = m.value("lowerSectionBorderZ", 0.0);
        meta.mapName = m.value("mapName", "");
        meta.mapScale = m.value("mapScale", 1.0);
        meta.mapOffset = std::pair(m.value("posX", 0), m.value("posY", 0));
        meta.roundFreezeEndTick = m.value("roundFreezeEndTick", 0);
        meta.roundNumber = m.value("roundNumber", 0);
        meta.roundScoreUpdateTick = m.value("roundScoreUpdateTick", 0);
        meta.roundTime = m.value("roundTime", 0);
        meta.score = std::pair(m["score"][0].get<int>(), m["score"][1].get<int>());
        meta.tickRate = m.value("tickRate", 64);
        meta.toTick = m.value("toTick", 0);
        meta.winReason = m.value("winReason", 0);
        meta.winnerTeam = m.value("winnerTeam", 0);
    } else {
        throw std::runtime_error("JSON does not contain \"meta\" field.");
    }

    if (j.contains("playerPositions")) {
        const json &playerPositions = j["playerPositions"];
        for (auto it = playerPositions.begin(); it != playerPositions.end(); ++it) {
            uint64_t steamid = std::stoull(it.key());
            const json &playerInfo = it.value();
            std::string name = playerInfo.value("name", "Unknown");
            int deathTick = playerInfo.value("deathTick", -1);

            if (playerInfo.contains("frames")) {
                const json &frames = playerInfo["frames"];
                Player p(steamid, name, deathTick, frames);
                players[steamid] = p;
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
              << "Score: " << meta.score.first << " - " << meta.score.second << "\n"
              << "--------------------------" << std::endl;

    for (const auto &entry : players) {
        entry.second.print();
    }
}

Round createRound(const std::string& filePath) {
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

    return {};
}
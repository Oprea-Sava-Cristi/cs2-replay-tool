#ifndef ROUND_HPP
#define ROUND_HPP

#include "Player.hpp"
#include <string>
#include <vector>
#include <unordered_map>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

struct Meta {
    int fromTick;
    bool isTwoFloor;
    double lowerSectionBorderZ;
    std::string mapName;
    double mapScale;
    int roundFreezeEndTick;
    int roundNumber;
    int roundScoreUpdateTick;
    int roundTime;
    std::vector<int> score;
    int tickRate;
    int toTick;
    int winReason;
    int winnerTeam;
};

class Round {
private:
    Meta meta;
    std::unordered_map<uint64_t, Player> playerPositions;

public:
    Round();
    explicit Round(const json &j);
    void print() const;
    std::string getMapName() const;
};

#endif // ROUND_HPP
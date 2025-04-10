#ifndef DEMO_HPP
#define DEMO_HPP

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

class Demo {
private:
    Meta meta;
    std::unordered_map<uint64_t, Player> playerPositions;

public:
    Demo();
    explicit Demo(const json &j);
    void print() const;
};

#endif // DEMO_HPP

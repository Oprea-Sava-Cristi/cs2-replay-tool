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
    std::pair<int, int> mapOffset;
    int roundFreezeEndTick;
    int roundNumber;
    int roundScoreUpdateTick;
    int roundTime;
    std::pair<int, int> score;
    int tickRate;
    int toTick;
    int winReason;
    int winnerTeam;
};

class Round {
private:
    Meta meta;
    std::unordered_map<uint64_t, Player> players;

public:
    Round();
    explicit Round(const json &j);

    void print() const;

    std::string getMapName() const { return meta.mapName; }
    double getMapScale() const { return meta.mapScale; }
    std::pair<int, int> getMapOffset() const { return meta.mapOffset; }
    int getStartTick() const { return meta.roundFreezeEndTick; }
    int getEndTick() const { return meta.roundScoreUpdateTick; }
    int getTickRate() const { return meta.tickRate; }
    std::unordered_map<uint64_t, Player>& getPlayers() { return players; }
};

Round createRound(const std::string&);

#endif // ROUND_HPP
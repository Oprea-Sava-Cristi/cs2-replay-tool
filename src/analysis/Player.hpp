#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <vector>
#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

struct TickData {
    unsigned char team;
    unsigned char health;
    unsigned char armor;
    std::vector<std::string> weapons;
    std::string activeWeapon;
    double X;
    double Y;
    double Z;
    bool isDucking;
    bool isPlanting;
    bool isReloading;
};

class Player {
private:
    uint64_t steamid;
    std::string name;
    std::vector<TickData> ticks;

public:
    Player();

    Player(uint64_t steamid, std::string name, const TickData &tickData);

    Player(uint64_t steamid, std::string name, const json &frames);

    ~Player();

    void print() const;
};

#endif // PLAYER_HPP

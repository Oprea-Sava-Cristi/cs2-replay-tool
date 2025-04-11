#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <vector>
#include <string>
#include <nlohmann/json.hpp>

#include "SFML/Graphics/CircleShape.hpp"

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
    sf::CircleShape playerCircle;

public:
    Player();

    Player(uint64_t steamid, std::string name, const TickData &tick);

    Player(uint64_t steamid, std::string name, const json &frames);

    ~Player();

    void updatePlayerCircle(const TickData &tick, double scale);

    TickData getTick(size_t);

    sf::CircleShape getPlayerCircle();

    void print() const;
};

#endif // PLAYER_HPP

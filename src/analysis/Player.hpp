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
    int deathTick;
    std::vector<TickData> ticks;
    sf::CircleShape playerCircle;

public:
    Player();

    Player(uint64_t steamid, std::string name, int deathTick, const TickData &tick);

    Player(uint64_t steamid, std::string name, int deathTick, const json &frames);

    ~Player();

    void updatePlayerCircle(const TickData &tick, double scale);

    void updatePlayerCircle(const sf::Texture& texture);

    std::optional<TickData> getTick(size_t);

    sf::CircleShape getPlayerCircle();

    int getPlayerInfo() const;

    void print() const;
};

#endif // PLAYER_HPP

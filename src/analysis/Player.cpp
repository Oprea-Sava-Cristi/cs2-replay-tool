#include "Player.hpp"
#include <iostream>
#include <utility>

#include "SFML/Graphics/Texture.hpp"

Player::Player()
    : steamid(0), name("Unknown"), deathTick(-1) {
    playerCircle = sf::CircleShape(8.f);
    playerCircle.setOrigin({8.f, 8.f});
}

Player::Player(uint64_t steamid, std::string name, int deathTick, const TickData &tick)
    : steamid(steamid), name(std::move(name)), deathTick(deathTick)  {
    ticks.push_back(tick);
    playerCircle = sf::CircleShape(8.f);
    playerCircle.setOrigin({8.f, 8.f});
}

Player::Player(uint64_t steamid, std::string name, int deathTick, const json &frames)
    : steamid(steamid), name(std::move(name)), deathTick(deathTick) {
    for (const auto &frame : frames) {
        TickData tick;
        tick.team = frame.value("team", 0);
        tick.health = frame.value("health", 0);
        tick.armor = frame.value("armor", 0);
        tick.weapons = frame.value("weapons", std::vector<std::string>{"Knife"});
        tick.activeWeapon = frame.value("activeWeapon", std::string("Knife"));
        tick.X = frame.value("positionX", 0.0);
        tick.Y = frame.value("positionY", 0.0);
        tick.Z = frame.value("positionZ", 0.0);
        tick.isDucking = frame.value("isDucking", false);
        tick.isPlanting = frame.value("isPlanting", false);
        tick.isReloading = frame.value("isReloading", false);
        ticks.push_back(tick);
    }
    playerCircle = sf::CircleShape(8.f);
    playerCircle.setOrigin({8.f, 8.f});
}

Player::~Player() = default;

void Player::updatePlayerCircle(const TickData &tick, double scale) {
    // std::cout << (tick.X-3230)/scale << " " << (tick.Y+1713)/scale << " " << scale << std::endl;
    playerCircle.setPosition({static_cast<float>((-(-3230)+tick.X)/scale), static_cast<float>((1713-tick.Y)/scale)});
    if (tick.team == 2) {
        playerCircle.setFillColor(sf::Color::Yellow);
    } else if (tick.team == 3) {
        playerCircle.setFillColor(sf::Color::Blue);
    } else {
        playerCircle.setFillColor(sf::Color::White);
    }
}

void Player::updatePlayerCircle(const sf::Texture& texture) {
    playerCircle.setTexture(&texture);
}

std::optional<TickData> Player::getTick(size_t index) {
    if (index < ticks.size()) {
        return ticks[index];
    }
    return std::nullopt;
}

sf::CircleShape Player::getPlayerCircle() {
    return playerCircle;
}

int Player::getPlayerInfo() const {
    // add the rest of the fields
    return deathTick;
}

void Player::print() const {
    std::cout << "Player Name: " << name << "\n"
              << "SteamID: " << steamid << "\n";
    int tickIndex = 0;
    for (const auto &tick : ticks) {
        std::cout << "Tick " << tickIndex++ << ":\n"
                  << "  Team: " << static_cast<int>(tick.team) << "\n"
                  << "  Health: " << static_cast<int>(tick.health) << "\n"
                  << "  Armor: " << static_cast<int>(tick.armor) << "\n"
                  << "  Active Weapon: " << tick.activeWeapon << "\n"
                  << "  Weapons: ";
        for (const auto &weapon : tick.weapons) {
            std::cout << weapon << " ";
        }
        std::cout << "\n"
                  << "  Position: (" << tick.X << ", " << tick.Y << ", " << tick.Z << ")\n"
                  << "  Is Ducking: " << std::boolalpha << tick.isDucking << "\n"
                  << "  Is Planting: " << std::boolalpha << tick.isPlanting << "\n"
                  << "  Is Reloading: " << std::boolalpha << tick.isReloading << "\n";
    }
    std::cout << "--------------------------" << std::endl;
}

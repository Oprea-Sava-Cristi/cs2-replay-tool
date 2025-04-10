#include "Player.hpp"
#include <iostream>
#include <utility>

Player::Player()
    : steamid(0), name("Unknown") {
}

Player::Player(uint64_t steamid, std::string name, const TickData &tickData)
    : steamid(steamid), name(std::move(name)) {
    ticks.push_back(tickData);
}

Player::Player(uint64_t steamid, std::string name, const json &frames)
    : steamid(steamid), name(std::move(name)) {
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
}

Player::~Player() = default;

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

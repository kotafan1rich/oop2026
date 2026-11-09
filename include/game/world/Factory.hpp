#pragma once

#include <random>

#include "game/units/Robot.hpp"

constexpr int defaultSpawnInterval = 10;

struct RobotConfig {
    int attack = 5;
    int speed = 1;
    int maxEnergy = 10;
    int maxHealth = 20;
    AbilityType ability = AbilityType::Heal;
};

class Factory {
   private:
    RobotConfig robotConfig_;
    Point point_ = Point(0, 0);
    int height_;
    int width_;
    int spawnInterval_ = defaultSpawnInterval;
    int stepCounter_ = 0;

   public:
    Factory(RobotConfig robotConfig, int height = 2, int width = 2);
    ~Factory();

    void setX(int x);
    void setY(int y);
    int getX() const;
    int getY() const;
    Point& getPosition();
    const Point& getPosition() const;
    void setPosition(const Point& point);

    void setSpawnInterval(int interval);
    int getSpawnInterval() const;

    int getHeight() const;
    int getWidth() const;

    AbilityType getRandomAbility(std::mt19937& generator);

    std::unique_ptr<Robot> spawn(std::mt19937& generator);

    void onTurnEnd();
};

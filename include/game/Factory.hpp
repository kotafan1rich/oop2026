#pragma once
#include <memory>

#include "game/Robot.hpp"

constexpr int defaultSpawnInterval = 10;

class Factory {
   private:
    Robot spawningRobot_;
    Point point_ = Point(0, 0);
    int height_;
    int width_;
    int spawnInterval_ = defaultSpawnInterval;
    int stepCounter_ = 1;

   public:
    Factory(Robot spawningRobot, int height = 2, int width = 2);
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

    std::unique_ptr<Robot> spawn();

    virtual void onTurnEnd();
};

#pragma once

#include <functional>
#include <vector>

#include "game/navigation/PathFinder.hpp"
#include "game/units/Player.hpp"
#include "game/units/Robot.hpp"
#include "game/world/Cell.hpp"
#include "game/world/Factory.hpp"

constexpr int maxHeight = 100;
constexpr int maxWidth = 100;
constexpr int minHeight = 5;
constexpr int minWidth = 5;

enum class MoveResult {
    Blocked,
    Moved,
    Interacted,
};

class World {
   private:
    int height_;
    int width_;
    std::vector<std::vector<Cell>> field_;
    std::vector<std::unique_ptr<Robot>> robots_;
    std::vector<std::unique_ptr<Factory>> factories_;
    std::unique_ptr<Player> player_;
    PathFinder pathFinder_;

    void initDefaultLevel();
    bool moveRobotUsingSpeed(
        Robot& robot,
        const std::function<Point(const Robot&)>& selectNextPoint);

   public:
    World(int height, int width);
    ~World();

    int getWidth() const;
    int getHeight() const;

    const Cell& getCell(const Point& point) const;
    Cell& getCell(const Point& point);

    void addRobot(std::unique_ptr<Robot> robot);
    MoveResult moveRobot(Robot& robot, int dx, int dy);
    bool moveRobotWithSpeed(Robot& robot, int dx, int dy);
    void killRobot(Robot& robot);
    const std::vector<std::unique_ptr<Robot>>& getRobots() const;
    Robot& findRobotAt(const Point& point);
    bool hasRobotAt(const Point& point) const;

    void moveBots();

    void addFactory(std::unique_ptr<Factory> factory, const Point& point);
    const std::vector<std::unique_ptr<Factory>>& getFactories() const;

    void addPlayer(std::unique_ptr<Player> player);
    Player& getPlayer();
    const Player& getPlayer() const;
    bool movePlayer(int dx, int dy);

    bool isInside(const Point& point) const;

    const std::vector<int>& getDistanceMap();

    void markSeen();

    bool endTurn();
};

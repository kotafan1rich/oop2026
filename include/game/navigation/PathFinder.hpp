#pragma once

#include <vector>

#include "game/world/Point.hpp"

class World;

class PathFinder {
   private:
    const World& world_;
    std::vector<int> distances_;
    Point target_ = Point(0, 0);
    bool ready_ = false;

   public:
    explicit PathFinder(const World& world);

    void buildDistanceMap(const Point& target);
    void buildDistanceMap(const Point& target, const Point& start);
    Point getNextPoint(const Point& from) const;
    Point getNextPointInRadius(const Point& from, int radius) const;

    const std::vector<int>& getDistanceMap();
};

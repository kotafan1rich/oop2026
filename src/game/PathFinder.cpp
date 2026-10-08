#include "game/PathFinder.hpp"

#include <algorithm>
#include <functional>
#include <limits>
#include <queue>
#include <utility>

#include "game/World.hpp"

PathFinder::PathFinder(const World& world) : world_(world) {}

void PathFinder::buildDistanceMap(const Point& target) {
    buildDistanceMap(target, target);
}

void PathFinder::buildDistanceMap(const Point& target, const Point& start) {
    ready_ = false;
    if (!world_.isInside(target) || !world_.isInside(start) ||
        !world_.getCell(target).isWalkable()) {
        return;
    }

    const int width = world_.getWidth();
    const int height = world_.getHeight();
    const int targetIndex = target.getY() * width + target.getX();

    distances_.assign(width * height, std::numeric_limits<int>::max());

    target_ = target;
    distances_[targetIndex] = 0;

    using QueueEntry = std::pair<int, Point>;
    auto greaterDistance = [](const QueueEntry& left, const QueueEntry& right) {
        return left.first > right.first;
    };
    std::priority_queue<QueueEntry, std::vector<QueueEntry>,
                        decltype(greaterDistance)>
        points(greaterDistance);
    points.emplace(0, target);

    constexpr int directions[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
    while (!points.empty()) {
        const auto [currentDistance, current] = points.top();
        points.pop();
        const int currentIndex = current.getY() * width + current.getX();
        if (currentDistance != distances_[currentIndex]) {
            continue;
        }

        for (const auto& direction : directions) {
            const Point nextPoint(current.getX() + direction[0],
                                  current.getY() + direction[1]);

            if (!world_.isInside(nextPoint)) {
                continue;
            }

            const int nextIndex = nextPoint.getY() * width + nextPoint.getX();
            const Cell& cell = world_.getCell(nextPoint);
            const bool occupiedByRobot = cell.getRobot() != nullptr;
            if (!cell.isWalkable() ||
                (occupiedByRobot && nextPoint != target &&
                 nextPoint != start)) {
                continue;
            }

            const int nextDistance =
                currentDistance + world_.getCell(current).getMovementCost();
            if (nextDistance < distances_[nextIndex]) {
                distances_[nextIndex] = nextDistance;
                points.emplace(nextDistance, nextPoint);
            }
        }
    }

    ready_ = true;
}

Point PathFinder::getNextPoint(const Point& from) const {
    if (!ready_ || !world_.isInside(from)) {
        return from;
    }

    const int width = world_.getWidth();
    const int fromIndex = from.getY() * width + from.getX();
    const int distanceFromStart = distances_[fromIndex];
    int bestDistance = std::numeric_limits<int>::max();
    Point bestPoint = from;

    if (distanceFromStart == std::numeric_limits<int>::max()) {
        return from;
    }

    constexpr int directions[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
    for (const auto& direction : directions) {
        const Point nextPoint(from.getX() + direction[0],
                              from.getY() + direction[1]);
        if (!world_.isInside(nextPoint)) {
            continue;
        }

        const Cell& cell = world_.getCell(nextPoint);
        if (!cell.isWalkable() || (cell.isTaken() && nextPoint != target_)) {
            continue;
        }

        const int nextIndex = nextPoint.getY() * width + nextPoint.getX();
        const int distanceFromNext = distances_[nextIndex];
        if (distanceFromNext == std::numeric_limits<int>::max()) {
            continue;
        }

        const int nextDistance = cell.getMovementCost() + distanceFromNext;
        if (nextDistance <= distanceFromStart && nextDistance < bestDistance) {
            bestDistance = nextDistance;
            bestPoint = nextPoint;
        }
    }

    return bestPoint;
}

const std::vector<int>& PathFinder::getDistanceMap() {
    return distances_;
}

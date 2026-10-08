#include "game/World.hpp"

#include <algorithm>
#include <cmath>
#include <exception>
#include <queue>

#include "game/Cell.hpp"
#include "game/PathFinder.hpp"

void World::initDefaultLevel() {
    if (width_ >= 12 && height_ >= 12) {
        const int leftWall = width_ / 3;
        const int rightWall = width_ * 2 / 3;
        const int middleRow = height_ / 2;
        const int topPassage = height_ / 4;
        const int bottomPassage = height_ * 3 / 4;
        const int leftPassage = width_ / 4;
        const int rightPassage = width_ * 3 / 4;

        auto makeObstacle = [this](int x, int y) {
            Cell& cell = field_[y][x];
            cell.setMovementCost(impassableMovementCost);
            cell.setTerrainType(TerrainType::Wall);
        };

        for (int y = 2; y < height_ - 2; ++y) {
            if (y != topPassage && y != topPassage + 1 && y != middleRow &&
                y != bottomPassage) {
                makeObstacle(leftWall, y);
            }

            if (y != topPassage && y != middleRow && y != bottomPassage - 1 &&
                y != bottomPassage) {
                makeObstacle(rightWall, y);
            }
        }

        for (int x = 2; x < width_ - 2; ++x) {
            if (x != leftPassage && x != leftPassage + 1 && x != leftWall &&
                x != width_ / 2 && x != rightWall && x != rightPassage &&
                x != rightPassage + 1) {
                makeObstacle(x, middleRow);
            }
        }

        const int upperRow = height_ / 4;
        for (int x = leftWall + 3; x < rightWall - 2; ++x) {
            if (x != width_ / 2) {
                makeObstacle(x, upperRow);
            }
        }

        const int lowerRow = height_ * 3 / 4;
        for (int x = leftWall + 2; x < rightWall - 2; ++x) {
            if (x != width_ / 2 && x != width_ / 2 + 1) {
                makeObstacle(x, lowerRow);
            }
        }

        auto makeSwamp = [this](int x, int y) {
            Cell& cell = field_[y][x];
            if (cell.isWalkable()) {
                cell.setMovementCost(swampMovementCost);
                cell.setTerrainType(TerrainType::Swamp);
            }
        };

        for (int y = topPassage + 2; y < middleRow - 1; ++y) {
            for (int x = 2; x < leftWall - 1; ++x) {
                if ((x + y) % 3 != 0) {
                    makeSwamp(x, y);
                }
            }
        }

        for (int y = middleRow + 2; y < bottomPassage - 1; ++y) {
            for (int x = leftWall + 2; x < rightWall - 2; ++x) {
                if ((x + 2 * y) % 4 != 0) {
                    makeSwamp(x, y);
                }
            }
        }

        for (int y = 2; y < topPassage - 1; ++y) {
            for (int x = rightWall + 2; x < width_ - 2; ++x) {
                if ((2 * x + y) % 3 != 0) {
                    makeSwamp(x, y);
                }
            }
        }
    }
}

World::World(int height, int width) : pathFinder_(*this) {
    height = std::min(maxHeight, height);
    height_ = std::max(minHeight, height);

    width = std::min(maxWidth, width);
    width_ = std::max(minWidth, width);

    field_ = std::vector<std::vector<Cell>>(
        height_, std::vector<Cell>(width_, Cell(true)));

    for (int x = 0; x < width_; ++x) {
        field_[0][x].setMovementCost(impassableMovementCost);
        field_[0][x].setTerrainType(TerrainType::Wall);
        field_[height_ - 1][x].setMovementCost(impassableMovementCost);
        field_[height_ - 1][x].setTerrainType(TerrainType::Wall);
    }

    for (int y = 0; y < height_; ++y) {
        field_[y][0].setMovementCost(impassableMovementCost);
        field_[y][0].setTerrainType(TerrainType::Wall);
        field_[y][width_ - 1].setMovementCost(impassableMovementCost);
        field_[y][width_ - 1].setTerrainType(TerrainType::Wall);
    }

    initDefaultLevel();
}

World::~World() {}

int World::getWidth() const { return width_; }

int World::getHeight() const { return height_; }

const Cell& World::getCell(const Point& point) const {
    if (!isInside(point)) {
        throw std::out_of_range("Point is outside");
    }
    return field_[point.getY()][point.getX()];
}

Cell& World::getCell(const Point& point) {
    if (!isInside(point)) {
        throw std::out_of_range("Point is outside");
    }
    return field_[point.getY()][point.getX()];
}

void World::addRobot(std::unique_ptr<Robot> robot) {
    if (!robot) {
        return;
    }
    Cell& cell = getCell(robot->getPosition());
    if (cell.isWalkable() && !cell.isTaken()) {
        cell.setRobot(robot.get());
        robots_.push_back(std::move(robot));
    }
}

MoveResult World::moveRobot(Robot& robot, int dx, int dy) {
    Point nextPoint(robot.getX() + dx, robot.getY() + dy);

    if (!isInside(nextPoint)) {
        return MoveResult::Blocked;
    }

    Cell& nextCell = getCell(nextPoint);

    if (!nextCell.isWalkable()) {
        return MoveResult::Blocked;
    }

    Robot* other = nextCell.getRobot();

    if (other) {
        bool isDead = robot.interact(*other);

        if (isDead) {
            killRobot(*other);
        }

        return MoveResult::Interacted;
    }

    getCell(robot.getPosition()).removeRobot();
    robot.setPosition(nextPoint);
    nextCell.setRobot(&robot);

    return MoveResult::Moved;
}

bool World::moveRobotWithSpeed(Robot& robot, int dx, int dy) {
    return moveRobotUsingSpeed(robot, [dx, dy](const Robot& movingRobot) {
        return Point(movingRobot.getX() + dx, movingRobot.getY() + dy);
    });
}

bool World::moveRobotUsingSpeed(
    Robot& robot, const std::function<Point(const Robot&)>& selectNextPoint) {
    bool isMoved = false;
    int movementPoints = robot.getSpeed();

    while (movementPoints > 0) {
        const Point nextPoint = selectNextPoint(robot);
        if (nextPoint == robot.getPosition() || !isInside(nextPoint)) {
            break;
        }

        const Cell& nextCell = getCell(nextPoint);
        const int movementCost = nextCell.getMovementCost();
        if (!nextCell.isWalkable() || movementCost > movementPoints ||
            movementCost > robot.getEnergy()) {
            break;
        }

        const int dx = nextPoint.getX() - robot.getX();
        const int dy = nextPoint.getY() - robot.getY();
        const MoveResult moved = moveRobot(robot, dx, dy);
        if (moved == MoveResult::Blocked) {
            break;
        }

        movementPoints -= movementCost;
        robot.expendEnergy(movementCost);
        isMoved = true;

        if (moved == MoveResult::Interacted) {
            break;
        }
    }

    return isMoved;
}

void World::killRobot(Robot& robot) {
    Robot* target = getCell(robot.getPosition()).removeRobot();

    std::erase_if(robots_, [target](const std::unique_ptr<Robot>& pointer) {
        return pointer.get() == target;
    });
}

const std::vector<std::unique_ptr<Robot>>& World::getRobots() const {
    return robots_;
}

void World::moveBots() {
    if (!player_ || player_->isDead()) {
        return;
    }

    for (auto& robot : getRobots()) {
        if (!robot || robot->isDead()) {
            continue;
        }
        if (robot->getTeam() == Team::Friend) {
            continue;
        }

        pathFinder_.buildDistanceMap(player_->getPosition(),
                                     robot->getPosition());
        moveRobotUsingSpeed(*robot, [this](const Robot& movingRobot) {
            return pathFinder_.getNextPoint(movingRobot.getPosition());
        });
    }
}

void World::addFactory(std::unique_ptr<Factory> factory, const Point& point) {
    if (!factory) {
        return;
    }

    for (int dy = 0; dy < factory->getHeight(); dy++) {
        for (int dx = 0; dx < factory->getWidth(); dx++) {
            Point cellPoint(point.getX() + dx, point.getY() + dy);

            if (!isInside(cellPoint)) {
                return;
            }

            Cell& currentCell = getCell(cellPoint);
            if (currentCell.isTaken() || !currentCell.isWalkable()) {
                return;
            }
        }
    }
    factory->setPosition(point);

    for (int dy = 0; dy < factory->getHeight(); dy++) {
        for (int dx = 0; dx < factory->getWidth(); dx++) {
            Point cellPoint(point.getX() + dx, point.getY() + dy);
            getCell(cellPoint).setFactory(factory.get());
        }
    }
    factories_.push_back(std::move(factory));
}

const std::vector<std::unique_ptr<Factory>>& World::getFactories() const {
    return factories_;
}

void World::addPlayer(std::unique_ptr<Player> player) {
    if (!player) {
        throw std::logic_error("Player cannot be nullptr");
    }
    Cell& cell = getCell(player->getPosition());
    if (cell.isWalkable() && !cell.isTaken()) {
        cell.setRobot(player.get());
        player_ = std::move(player);
        return;
    }
    throw std::logic_error("Failed to add player");
}

Player& World::getPlayer() {
    if (!player_) {
        throw std::logic_error("World has no player");
    }
    return *player_;
}

const Player& World::getPlayer() const {
    if (!player_) {
        throw std::logic_error("World has no player");
    }
    return *player_;
}

bool World::movePlayer(int dx, int dy) {
    if (!player_ || player_->isDead()) {
        return false;
    }
    Player& player = getPlayer();
    return moveRobotWithSpeed(player, dx, dy);
}

bool World::isInside(const Point& point) const {
    return point.getX() >= 0 && point.getX() < width_ && point.getY() >= 0 &&
           point.getY() < height_;
}

const std::vector<int>& World::getDistanceMap() {
    pathFinder_.buildDistanceMap(getPlayer().getPosition());
    return pathFinder_.getDistanceMap();
}

void World::markSeen() {
    Player& player = getPlayer();
    Point& playerPos = player.getPosition();

    constexpr int directions[4][2] = {{1, 1}, {-1, 1}, {1, -1}, {-1, -1}};
    for (int y = 0; y <= player.getVisibiltyRadius(); y++) {
        for (int x = 0; x <= player.getVisibiltyRadius(); x++) {
            for (auto direction : directions) {
                int newX = playerPos.getX() + x * direction[0];
                int newY = playerPos.getY() + y * direction[1];
                Point currentPoint(newX, newY);
                if (isInside(currentPoint) && player.isInRadius(currentPoint)) {
                    getCell(currentPoint).markSeen();
                }
            }
        }
    }
}

bool World::endTurn() {
    for (auto& robot : robots_) {
        if (robot) {
            robot->onTurnEnd();
        }
    }
    for (auto& factory : factories_) {
        if (factory) {
            Point spawnPoint(factory->getX() - 1, factory->getY());
            if (isInside(spawnPoint) && !getCell(spawnPoint).isTaken()) {
                auto newRobot = factory->spawn();
                if (newRobot) {
                    newRobot->setPosition(spawnPoint);
                    addRobot(std::move(newRobot));
                }
            }
        }
        factory->onTurnEnd();
    }

    if (player_) {
        player_->onTurnEnd();
    }
    const bool hasEnemies = std::any_of(
        robots_.begin(), robots_.end(), [](const auto& robot) {
            return robot && !robot->isDead() &&
                   robot->getTeam() == Team::Enemy;
        });
    if (!hasEnemies || getPlayer().isDead()) {
        return true;
    }
    return false;
}

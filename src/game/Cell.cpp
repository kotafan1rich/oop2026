#include "game/Cell.hpp"

#include "game/Point.hpp"

Cell::Cell(int movementCost) : movementCost_(movementCost) {}

Cell::~Cell() {}

TerrainType Cell::getTerrainType() const { return terrainType_; }

void Cell::setTerrainType(TerrainType type) { terrainType_ = type; }

bool Cell::isWalkable() const {
    return movementCost_ > impassableMovementCost && getFactory() == nullptr;
}

int Cell::getMovementCost() const { return movementCost_; }

void Cell::setMovementCost(int movementCost) {
    if (movementCost >= impassableMovementCost) {
        movementCost_ = movementCost;
    }
}

void Cell::setRobot(Robot* robot) { robot_ = robot; }

Robot* Cell::getRobot() const { return robot_; }

Robot* Cell::removeRobot() {
    Robot* robot = robot_;
    robot_ = nullptr;
    return robot;
}

void Cell::setFactory(Factory* factory) {
    if (!isTaken()) {
        factory_ = factory;
    }
}

Factory* Cell::getFactory() const { return factory_; }

Factory* Cell::removeFactory() {
    Factory* result = getFactory();
    factory_ = nullptr;
    return result;
}

void Cell::markSeen() { isSeen_ = true; }

bool Cell::isSeen() const { return isSeen_; }

bool Cell::isTaken() const {
    return getRobot() != nullptr || getFactory() != nullptr;
}

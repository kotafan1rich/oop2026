#include "game/world/Cell.hpp"

Cell::Cell(int movementCost) : movementCost_(movementCost) {}

Cell::~Cell() {}

TerrainType Cell::getTerrainType() const { return terrainType_; }

void Cell::setTerrainType(TerrainType type) { terrainType_ = type; }

bool Cell::isWalkable() const {
    return movementCost_ > impassableMovementCost &&
           getOccupantType() != OccupantType::Factory;
}

int Cell::getMovementCost() const { return movementCost_; }

void Cell::setMovementCost(int movementCost) {
    if (movementCost >= impassableMovementCost) {
        movementCost_ = movementCost;
    }
}

OccupantType Cell::getOccupantType() const { return occupant_; }

void Cell::setOccupantType(OccupantType occupant) { occupant_ = occupant; }

void Cell::clearOccupant() { occupant_ = OccupantType::Empty; }

const std::vector<StatusConfig>& Cell::statuses() const { return statuses_; }

void Cell::addStatus(StatusConfig status) { statuses_.push_back(status); }

void Cell::removeRandomStatus(std::mt19937& generator) {
    if (statuses_.empty()) {
        return;
    }
    std::uniform_int_distribution<int> choice(0, statuses_.size() - 1);
    statuses_.erase(statuses_.begin() + choice(generator));
}

void Cell::markSeen() { isSeen_ = true; }

bool Cell::isSeen() const { return isSeen_; }

bool Cell::isTaken() const { return occupant_ != OccupantType::Empty; }

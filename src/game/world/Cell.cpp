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
    std::vector<int> candidates;
    for (int i = 0; i < statuses_.size(); ++i) {
        if (statuses_[i].stausType != StatusType::ItemPickup) {
            candidates.push_back(i);
        }
    }
    if (candidates.empty()) {
        return;
    }
    std::uniform_int_distribution<int> choice(0, candidates.size() - 1);
    statuses_.erase(statuses_.begin() + candidates[choice(generator)]);
}

void Cell::removeStatus(StatusType type) {
    for (auto it = statuses_.begin(); it != statuses_.end(); ++it) {
        if (it->stausType == type) {
            statuses_.erase(it);
            return;
        }
    }
}

bool Cell::hasStatus() const { return !statuses_.empty(); }

void Cell::markSeen() { isSeen_ = true; }

bool Cell::isSeen() const { return isSeen_; }

bool Cell::isTaken() const { return occupant_ != OccupantType::Empty; }

#pragma once

#include <random>
#include <vector>

#include "game/statuses/Status.hpp"

constexpr int defaultMovementCost = 1;
constexpr int swampMovementCost = 2;
constexpr int impassableMovementCost = 0;

enum class TerrainType { Floor, Wall, Swamp };

enum class OccupantType {
    Empty,
    Factory,
    Robot,
};

class Cell {
   private:
    TerrainType terrainType_ = TerrainType::Floor;
    int movementCost_;
    bool isSeen_ = false;
    OccupantType occupant_ = OccupantType::Empty;
    std::vector<StatusConfig> statuses_;

   public:
    Cell(int movementCost = defaultMovementCost);
    ~Cell();

    TerrainType getTerrainType() const;
    void setTerrainType(TerrainType type);

    bool isWalkable() const;

    int getMovementCost() const;
    void setMovementCost(int movementCost);

    OccupantType getOccupantType() const;
    void setOccupantType(OccupantType occupant);
    void clearOccupant();

    const std::vector<StatusConfig>& statuses() const;
    void addStatus(StatusConfig status);
    void removeRandomStatus(std::mt19937& generator);
    void removeStatus(StatusType type);
    bool hasStatus() const;

    void markSeen();
    bool isSeen() const;

    bool isTaken() const;
};
